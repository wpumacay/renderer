# pylint: skip-file
# -----------------------------
# Options affecting formatting.
# -----------------------------
with section("format"):

    # If a statement is wrapped to more than one line, then dangle the closing
    # parenthesis on its own line.
    dangle_parens = False

    # What style line endings to use in the output.
    line_ending = 'unix'

    # How wide to allow formatted cmake files
    line_width = 80

    # If a positional argument group contains more than this many arguments, then
    # force it to a vertical layout.
    max_pargs_hwrap = 7

    # If true, separate flow control names from their parentheses with a space
    separate_ctrl_name_with_space = False

    # If true, separate function names from parentheses with a space
    separate_fn_name_with_space = False

    # How many spaces to tab for indent
    tab_size = 2

    # If true, lines are indented using tab characters (utf-8 0x09) instead of
    # <tab_size> space characters (utf-8 0x20). In cases where the layout would
    # require a fractional tab character, the behavior of the  fractional
    # indentation is governed by <fractional_tab_policy>
    use_tabchars = False

    # Format command names consistently as 'lower' or 'upper' case
    command_case = 'unchanged'

# ------------------------------------------------
# Options affecting comment reflow and formatting.
# ------------------------------------------------
with section("markup"):

    # What character to use for bulleted lists
    bullet_char = '*'

    # What character to use as punctuation after numerals in an enumerated list
    enum_char = '.'

# ----------------------------
# Options affecting the linter
# ----------------------------
with section("lint"):

    # a list of lint codes to disable
    disabled_codes = []

    # regular expression pattern describing valid function names
    function_pattern = '[0-9a-zA-Z_]+'

    # regular expression pattern describing valid macro names
    macro_pattern = '[0-9a-zA-Z_]+'

    # regular expression pattern describing valid names for variables with global
    # (cache) scope
    global_var_pattern = '[A-Z][0-9A-Z_]+'

    # regular expression pattern describing valid names for variables with global
    # scope (but internal semantic)
    internal_var_pattern = '_[A-Z][0-9A-Z_]+'

    # regular expression pattern describing valid names for variables with local
    # scope
    local_var_pattern = '[a-z][a-z0-9_]+'

    # regular expression pattern describing valid names for privatedirectory
    # variables
    private_var_pattern = '_[0-9a-z_]+'

    # regular expression pattern describing valid names for public directory
    # variables
    public_var_pattern = '[A-Z][0-9A-Z_]+'

    # regular expression pattern describing valid names for function/macro
    # arguments and loop variables.
    argument_var_pattern = '[a-z][a-z0-9_]+'

    # regular expression pattern describing valid names for keywords used in
    # functions or macros
    keyword_pattern = '[A-Z][0-9A-Z_]+'

    # In the heuristic for C0201, how many conditionals to match within a loop in
    # before considering the loop a parser.
    max_conditionals_custom_parser = 2

    # Require at least this many newlines between statements
    min_statement_spacing = 1

    # Require no more than this many newlines between statements
    max_statement_spacing = 2
    max_returns = 6
    max_branches = 12
    max_arguments = 5
    max_localvars = 15
    max_statements = 50

# -------------------------------
# Options affecting file encoding
# -------------------------------
with section("encode"):

    # If true, emit the unicode byte-order mark (BOM) at the start of the file
    emit_byteorder_mark = False

    # Specify the encoding of the input file. Defaults to utf-8
    input_encoding = 'utf-8'

    # Specify the encoding of the output file. Defaults to utf-8. Note that cmake
    # only claims to support utf-8 so be careful when using anything else
    output_encoding = 'utf-8'
