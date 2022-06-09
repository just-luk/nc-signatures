#! /usr/bin/env python

import os
import argparse
import pandas as pd
import json
import jinja2
import textwrap
import altair as alt

# Add the plots as separate charts - so they can be downloaded individually
HTML_TEMPLATE = r"""
<!DOCTYPE html>
<html>
<head>
  <script src="https://cdn.jsdelivr.net/npm/vega@{{vega_version}}"></script>
  <script src="https://cdn.jsdelivr.net/npm/vega-lite@{{vegalite_version}}"></script>
  <script src="https://cdn.jsdelivr.net/npm/vega-embed@{{vegaembed_version}}"></script>
  <title>Fifi Benchmarks</title>
</head>
<body>

<p>
{{executable}}
<br>
Date: {{date}}
<br>
Host: {{host_name}}
<br>
CPU {{num_cpus}} cores @ {{mhz_per_cpu}}
<br>
CPU Scaling {% if cpu_scaling_enabled %}enabled{% else %}disabled{% endif %}.
<br>
Build type: {{library_build_type}}
</div>
{% for _ in charts -%}
<div id="vis{{loop.index}}"></div>
{% endfor -%}
<script type="text/javascript">
{% for chart in charts -%}
vegaEmbed('#vis{{loop.index}}', {{chart}}).catch(console.error);
{% endfor -%}
</script>
</body>
</html>
""".strip()


def add_byte_suffix(string: str):
    return string + " B"


def bps_to_mbps(bps: float):
    return bps / (1024 * 1024)


def clean_dataframe(df: pd.DataFrame):
    error_list = df["error_occurred"]
    checks = len(error_list)
    dropped_indices = []
    for i in range(checks):
        if error_list[i] == True:
            dropped_indices.append(i)
    df.drop(dropped_indices, inplace=True)
    df.reset_index(inplace=True)

def plot(df, fields):
    for key, group in df.groupby(fields):

        title = key[0].replace("_", " ").title()

        config = textwrap.wrap(", ".join(
            [f"{f} = {k}" for f, k in zip(fields[1:], key[1:])])
        )

        chart = (
            alt.Chart(group)
            .mark_bar()
            .encode(
                x=alt.X("testcase:N", axis=alt.Axis(title="Test-case", labels=False)),
                y=alt.Y("throughput:Q", axis=alt.Axis(title="Throughput [MB/s]")),
                color=alt.Color("testcase:N", legend=alt.Legend(title="Test-case")),
            )
            .properties(title={"text": title, "subtitle": config}, width=200)
        )
        yield chart.to_json(indent=None)

def plot_operations(context, df: pd.DataFrame):
    """
    Plots the speed of the different stacks for the region arithmetic
    operations.

    :param df: The dataframe containing the results
    """

    benchmark_names = []
    method_names = []

    for i in range(len(df["name"])):
        name_indices = df["name"][i].split("/")
        benchmark_name = name_indices[0].replace("BM_", "")
        method_name = name_indices[1]
        benchmark_names.append(benchmark_name)
        method_names.append(method_name)

    df.drop(columns="name", inplace=True)
    df.insert(0, "benchmark", benchmark_names)
    df.insert(1, "testcase", method_names)

    df = df.rename(columns={"label": "field"})
    df = df.rename(columns={"bytes_per_second": "throughput"})

    df["throughput"] = df["throughput"].apply(bps_to_mbps)
    df["size"] = df["size"].apply(int).apply(str).apply(add_byte_suffix)

    df_1 = df[df["vectors"].isnull().values]
    df_2 = df[df["vectors"].notnull().values]

    df_1 = df_1[
        [
            "benchmark",
            "testcase",
            "field",
            "size",
            "throughput",
        ]
    ]

    df_2 = df_2[
        [
            "benchmark",
            "testcase",
            "field",
            "size",
            "throughput",
            "vectors",
        ]
    ]

    df_2["vectors"] = df_2["vectors"].apply(int)

    charts = []
    charts += plot(df_1, ["benchmark", "size", "field"])
    charts += plot(df_2, ["benchmark", "size", "field", "vectors"])
    
    chart_path = os.path.abspath(os.path.join(".", "chart.html"))

    with open(chart_path, "w") as f:
        template = jinja2.Template(HTML_TEMPLATE)
        output = template.render(
            vega_version=alt.VEGA_VERSION,
            vegalite_version=alt.VEGALITE_VERSION,
            vegaembed_version=alt.VEGAEMBED_VERSION,
            charts=charts,
            **context
        )

        f.write(output)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "-i",
        action="store",
        dest="input",
        required=True,
        help="The input .json file produced by the fifi arithmetic benchmark",
    )

    args = parser.parse_args()

    with open(args.input, "r") as file:
        data = json.load(file)

    results = data["benchmarks"]

    df = pd.DataFrame(results)

    clean_dataframe(df)
    plot_operations(data['context'], df.copy(deep=True))


if __name__ == "__main__":
    main()
