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
    "guzzle_sphinx_theme",
    "wurfapi",
]

# wurfapi options - relative to your docs dir
wurfapi = {
    "source_paths": [
        "doxygen_copydoc.hpp",
        # Fields
        "../src/fifi/field/binary.hpp",
        "../src/fifi/field/binary4.hpp",
        "../src/fifi/field/binary8.hpp",
        "../src/fifi/field/binary16.hpp",
        "../src/fifi/field/prime2325.hpp",
        # Accelerations
        "../src/fifi/field/binary_basic.hpp",
        "../src/fifi/field/binary_avx2.hpp",
        "../src/fifi/field/binary_sse2.hpp",
        "../src/fifi/field/binary_neon.hpp",
        "../src/fifi/field/binary4_basic.hpp",
        "../src/fifi/field/binary4_avx2.hpp",
        "../src/fifi/field/binary4_neon.hpp",
        "../src/fifi/field/binary4_ssse3.hpp",
        "../src/fifi/field/binary8_basic.hpp",
        "../src/fifi/field/binary8_avx2.hpp",
        "../src/fifi/field/binary8_neon.hpp",
        "../src/fifi/field/binary8_ssse3.hpp",
        "../src/fifi/field/binary16_basic.hpp",
        "../src/fifi/field/prime2325_basic.hpp",
        # Backends
        "../src/fifi/field/prime2325_backend.hpp",
        "../src/fifi/field/binary16_backend.hpp",
        "../src/fifi/field/binary8_backend.hpp",
        "../src/fifi/field/binary4_backend.hpp",
        "../src/fifi/field/binary_backend.hpp",
        # API
        "../src/fifi/finite_field.hpp",
        "../src/fifi/field_metric.hpp",
        "../src/fifi/simd.hpp",
        "../src/fifi/math.hpp",
        "../src/fifi/utils.hpp",
        "../src/fifi/to_field.hpp",
        "../src/fifi/to_string.hpp",
        "../src/fifi/version.hpp",
    ],
    "recursive": False,
    "user_templates": "rst_templates",
    "include_paths": ["../src"],
    "parser": {
        "type": "doxygen",
        "download": True,
        "warnings_as_error": True,
        "patch_api": [
            {"selector": "fifi::STEINWURF_FIFI_VERSION", "key": "inline", "value": True}
        ],
        "collapse_inline_namespaces": ["fifi::STEINWURF_FIFI_VERSION"],
    },
}

# Add any paths that contain templates here, relative to this directory.
templates_path = []

# The suffix(es) of source filenames
source_suffix = ".rst"

# The master toctree document.
master_doc = "index"

# General information about the project.
project = u"Fifi"
copyright = u"2018, Steinwurf"
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
exclude_patterns = ["rst_templates"]

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
#
# html_theme_options = {}

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
html_logo = "../fifi_icon_only.svg"

# The name of an image file (relative to this directory) to use as a favicon of
# the docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
#
# html_favicon = None

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
