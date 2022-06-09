# -*- coding: utf-8 -*-

# -- General configuration ------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.doctest",
    "sphinx.ext.intersphinx",
    "sphinx.ext.todo",
    "sphinx.ext.coverage",
    "sphinx.ext.mathjax",
    "sphinx.ext.ifconfig",
    "sphinx.ext.autosectionlabel",
    "guzzle_sphinx_theme",
    "wurfapi",
]

# wurfapi options - relative to your docs dir
wurfapi = {
    "source_paths": [
        # Block codes
        "../src/kodo/block/decoder.hpp",
        "../src/kodo/block/encoder.hpp",
        "../src/kodo/block/generator/parity_2d.hpp",
        "../src/kodo/block/generator/rs_cauchy.hpp",
        "../src/kodo/block/generator/random_uniform.hpp",
        "../src/kodo/block/generator/tunable.hpp",
        # Fulcrum codes
        "../src/kodo/fulcrum/decoder.hpp",
        "../src/kodo/fulcrum/encoder.hpp",
        "../src/kodo/fulcrum/generator/random_uniform.hpp",
        # Perpetual codes
        "../src/kodo/perpetual/decoder.hpp",
        "../src/kodo/perpetual/encoder.hpp",
        "../src/kodo/perpetual/width.hpp",
        "../src/kodo/perpetual/generator/random_uniform.hpp",
        "../src/kodo/perpetual/offset/random_uniform.hpp",
        # Sliding window codes
        "../src/kodo/slide/decoder.hpp",
        "../src/kodo/slide/encoder.hpp",
        "../src/kodo/slide/generator/random_uniform.hpp",
        "../src/kodo/slide/stream.hpp",
        "../src/kodo/slide/rate_controller.hpp",
        "../src/kodo/slide/range.hpp",
        # API
        "../src/kodo/version.hpp",
        "../src/kodo/finite_field.hpp",
        "../src/kodo/log_callback.hpp",
    ],
    "recursive": False,
    "include_paths": ["../src"],
    "parser": {
        "type": "doxygen",
        "download": True,
        "warnings_as_error": True,
        "patch_api": [
            {"selector": "kodo::STEINWURF_KODO_VERSION", "key": "inline", "value": True}
        ],
        "collapse_inline_namespaces": ["kodo::STEINWURF_KODO_VERSION"],
    },
}

# Add any paths that contain templates here, relative to this directory.
templates_path = []

# The suffix(es) of source filenames
source_suffix = ".rst"

# The master toctree document.
master_doc = "index"

# General information about the project.
project = u"Kodo"
copyright = u"2019, Steinwurf"
author = u"Steinwurf"

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = u""
# The full version, including alpha/beta/rc tags.
release = u""

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
# exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = "sphinx"

# -- Options for HTML output ----------------------------------------------

try:
    import guzzle_sphinx_theme

    html_theme = "guzzle_sphinx_theme"
    html_theme_path = guzzle_sphinx_theme.html_theme_path()
except ImportError:
    print(
        "Unable to import the used theme.\n"
        "Please install requirements.txt before building"
    )
    pass

html_sidebars = {
    "**": ["logo.html", "logo-text.html", "globaltoc.html", "searchbox.html"]
}

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
html_theme_options = {
    "h1_background_color": "a771b91A",
    "sidebar_hover_color": "a771b9aA",
    "logo_text_background_color": "a771b9",
    "link_color": "a771b9",
    "link_hover_color": "785185",
    "code_color": "6f16ab",
    "target_highlight_color": "a771b9",
    "highlighted_color": "d6afe3",
    "search_form_focus_color": "d6afe3",
}

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
html_logo = "images/icon.svg"

# The name of an image file (relative to this directory) to use as a favicon of
# the docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
html_favicon = "images/icon.ico"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = []

# Add any extra paths that contain custom files (such as robots.txt or
# .htaccess) here, relative to this directory. These files are copied
# directly to the root of the documentation.
#
# html_extra_path = []

# If true, the reST sources are included in the HTML build as _sources/name
html_copy_source = False

# If true, links to the reST sources are added to the pages.
html_show_sourcelink = False

# If true, prefix each section label with the name of the document it is in,
# followed by a colon.
autosectionlabel_prefix_document = True
