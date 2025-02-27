#include "Root Module.h"
namespace Jakt {
ErrorOr<void> write_to_file(const String data,const String output_filename) {
{
NonnullRefPtr<File> outfile = TRY((File::open_for_writing(output_filename)));
JaktInternal::Array<u8> bytes = (TRY((Array<u8>::create_with({}))));
{
JaktInternal::Range<size_t> _magic = (JaktInternal::Range<size_t>{static_cast<size_t>(static_cast<size_t>(0ULL)),static_cast<size_t>(((data).length()))});
for (;;){
JaktInternal::Optional<size_t> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
size_t i = (_magic_value.value());
{
TRY((((bytes).push(((data).byte_at(i))))));
}

}
}

TRY((((outfile)->write(bytes))));
}
return {};
}

ErrorOr<size_t> run_compiler(ParallelExecutionPool& pool,const String cxx_compiler_path,const String cpp_filename,const String output_filename,const String runtime_path,const JaktInternal::Array<String> extra_include_paths,const JaktInternal::Array<String> extra_lib_paths,const JaktInternal::Array<String> extra_link_libs,const bool optimize,const JaktInternal::Array<String> extra_compiler_flags) {
{
NonnullRefPtr<utility::FilePath> file_path = TRY((utility::FilePath::create(cxx_compiler_path)));
JaktInternal::Array<String> extra_flags = (TRY((Array<String>::create_with({}))));
if ((TRY((((file_path)->basename(false)))) == String("g++"))){
TRY((((extra_flags).push(String("-Wno-literal-suffix")))));
TRY((((extra_flags).push(String("-Wno-unused-parameter")))));
TRY((((extra_flags).push(String("-Wno-unused-but-set-variable")))));
TRY((((extra_flags).push(String("-Wno-unused-result")))));
TRY((((extra_flags).push(String("-Wno-implicit-fallthrough")))));
TRY((((extra_flags).push(String("-Wno-unused-command-line-argument")))));
}
{
JaktInternal::ArrayIterator<String> _magic = ((extra_compiler_flags).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String extra_flag = (_magic_value.value());
{
TRY((((extra_flags).push(extra_flag))));
}

}
}

JaktInternal::Array<String> compile_args = (TRY((Array<String>::create_with({cxx_compiler_path, String("-fdiagnostics-color=always"), String("-std=c++20"), String("-fno-exceptions"), String("-Wno-unknown-warning-option"), String("-Wno-trigraphs"), String("-Wno-parentheses-equality"), String("-Wno-unqualified-std-cast-call"), String("-Wno-user-defined-literals"), String("-Wno-deprecated-declarations")}))));
if (optimize){
TRY((((compile_args).push(String("-O3")))));
}
if ((!(((extra_flags).is_empty())))){
{
JaktInternal::ArrayIterator<String> _magic = ((extra_flags).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String flag = (_magic_value.value());
{
TRY((((compile_args).push(flag))));
}

}
}

}
TRY((((compile_args).push(String("-I")))));
TRY((((compile_args).push(runtime_path))));
TRY((((compile_args).push(String("-o")))));
TRY((((compile_args).push(output_filename))));
TRY((((compile_args).push(cpp_filename))));
if ((!(((extra_include_paths).is_empty())))){
TRY((((compile_args).add_capacity((JaktInternal::checked_mul<size_t>(((extra_include_paths).size()),static_cast<size_t>(2ULL)))))));
{
JaktInternal::ArrayIterator<String> _magic = ((extra_include_paths).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String path = (_magic_value.value());
{
TRY((((compile_args).push(String("-I")))));
TRY((((compile_args).push(path))));
}

}
}

}
if ((!(((extra_lib_paths).is_empty())))){
TRY((((compile_args).add_capacity((JaktInternal::checked_mul<size_t>(((extra_lib_paths).size()),static_cast<size_t>(2ULL)))))));
{
JaktInternal::ArrayIterator<String> _magic = ((extra_lib_paths).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String path = (_magic_value.value());
{
TRY((((compile_args).push(String("-L")))));
TRY((((compile_args).push(path))));
}

}
}

}
if ((!(((extra_link_libs).is_empty())))){
TRY((((compile_args).add_capacity(((extra_link_libs).size())))));
{
JaktInternal::ArrayIterator<String> _magic = ((extra_link_libs).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String path = (_magic_value.value());
{
TRY((((compile_args).push((String("-l") + path)))));
}

}
}

}
return (TRY((((((pool))).run(compile_args)))));
}
}

ErrorOr<String> indent(const size_t level) {
{
String output = String("");
{
JaktInternal::Range<size_t> _magic = (JaktInternal::Range<size_t>{static_cast<size_t>(static_cast<size_t>(0ULL)),static_cast<size_t>(level)});
for (;;){
JaktInternal::Optional<size_t> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
size_t i = (_magic_value.value());
{
(output += String("    "));
}

}
}

return (output);
}
}

ErrorOr<int> main(const JaktInternal::Array<String> args) {


            #ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
            setvbuf(stdout, nullptr, _IOFBF, 1000);
            #endif
{
if ((((args).size()) <= static_cast<size_t>(1ULL))){
warnln(String("{}"),usage());
return (static_cast<i64>(1LL));
}
utility::ArgsParser args_parser = TRY((utility::ArgsParser::from_args(args)));
if (TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-h"), String("--help")}))))))))){
outln(String("{}\n"),usage());
outln(String("{}"),help());
return (static_cast<i64>(0LL));
}
if (TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-v"), String("--version")}))))))))){
outln(String("unreleased"));
return (static_cast<i64>(0LL));
}
const NonnullRefPtr<utility::FilePath> current_executable_path = TRY((utility::FilePath::make(TRY((File::current_executable_path())))));
const String default_runtime_path = (TRY((((TRY((utility::FilePath::make(TRY((((current_executable_path)->dirname())))))))->dirname()))) + String("/include/runtime"));
const bool optimize = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-O")}))))))));
const bool lexer_debug = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-dl")}))))))));
const bool parser_debug = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-dp")}))))))));
const bool typechecker_debug = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-dt")}))))))));
const bool build_executable = (!(TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-S")}))))))))));
const bool run_executable = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-cr"), String("--compile-run")}))))))));
const bool codegen_debug = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-d")}))))))));
const bool debug_print = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("--debug-print")}))))))));
const bool prettify_cpp_source = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-p"), String("--prettify-cpp-source")}))))))));
const bool json_errors = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-j"), String("--json-errors")}))))))));
const bool dump_type_hints = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-H"), String("--type-hints")}))))))));
const bool dump_try_hints = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("--try-hints")}))))))));
const bool check_only = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-c"), String("--check-only")}))))))));
const bool write_source_to_file = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-S"), String("--emit-cpp-source-only")}))))))));
const JaktInternal::Optional<String> generate_depfile = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-M"), String("--dep-file")}))))))));
const JaktInternal::Optional<String> target_triple = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-T"), String("--target-triple")}))))))));
const String runtime_library_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-RLP"), String("--runtime-library-path")})))))))).value_or_lazy_evaluated([&] { return String("build/runtime"); });
const String compiler_job_count = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-J"), String("--jobs")})))))))).value_or_lazy_evaluated([&] { return String("2"); });
const String clang_format_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-F"), String("--clang-format-path")})))))))).value_or_lazy_evaluated([&] { return String("clang-format"); });
const String runtime_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-R"), String("--runtime-path")})))))))).value_or_lazy_evaluated([&] { return default_runtime_path; });
const String binary_dir = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-B"), String("--binary-dir")})))))))).value_or_lazy_evaluated([&] { return String("build"); });
const JaktInternal::Optional<String> dot_clang_format_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-D"), String("--dot-clang-format-path")}))))))));
const String cxx_compiler_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-C"), String("--cxx-compiler-path")})))))))).value_or_lazy_evaluated([&] { return String("clang++"); });
const JaktInternal::Optional<String> archiver_path = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-A"), String("--archiver")}))))))));
const JaktInternal::Optional<String> link_archive = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-a"), String("--link-archive")}))))))));
const JaktInternal::Array<String> extra_include_paths = TRY((((args_parser).option_multiple((TRY((Array<String>::create_with({String("-I")}))))))));
const JaktInternal::Array<String> extra_lib_paths = TRY((((args_parser).option_multiple((TRY((Array<String>::create_with({String("-L")}))))))));
const JaktInternal::Array<String> extra_link_libs = TRY((((args_parser).option_multiple((TRY((Array<String>::create_with({String("-l")}))))))));
const JaktInternal::Optional<String> set_output_filename = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-o"), String("--output-filename")}))))))));
const JaktInternal::Optional<String> goto_def = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-g"), String("--goto-def")}))))))));
const JaktInternal::Optional<String> goto_type_def = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-t"), String("--goto-type-def")}))))))));
const JaktInternal::Optional<String> hover = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-e"), String("--hover")}))))))));
const JaktInternal::Optional<String> completions = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-m"), String("--completions")}))))))));
const bool interpret_run = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-r"), String("--run")}))))))));
const bool format = TRY((((args_parser).flag((TRY((Array<String>::create_with({String("-f"), String("--format")}))))))));
const String input_format_range = TRY((((args_parser).option((TRY((Array<String>::create_with({String("-fr"), String("--format-range")})))))))).value_or_lazy_evaluated([&] { return String(""); });
const size_t max_concurrent = (infallible_integer_cast<size_t>((({ Optional<u32> __jakt_var_549;
auto __jakt_var_550 = [&]() -> ErrorOr<u32> { return TRY((value_or_throw<u32>(((compiler_job_count).to_uint())))); }();
if (__jakt_var_550.is_error()) {{
warnln(String("error: invalid value for --jobs: {}"),compiler_job_count);
return (static_cast<i64>(1LL));
}
} else {__jakt_var_549 = __jakt_var_550.release_value();
}
__jakt_var_549.release_value(); }))));
if (TRY((((args_parser).flag((TRY((Array<String>::create_with({String("--repl")}))))))))){
repl::REPL repl = TRY((repl::REPL::create(target_triple)));
TRY((((repl).run())));
return (static_cast<i64>(0LL));
}
const JaktInternal::Array<String> positional_arguments = TRY((((args_parser).remaining_arguments())));
JaktInternal::Optional<String> file_name = JaktInternal::OptionalNone();
bool first_arg = true;
JaktInternal::Array<String> interpreted_main_arguments = (TRY((Array<String>::create_with({}))));
{
JaktInternal::ArrayIterator<String> _magic = ((positional_arguments).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String arg = (_magic_value.value());
{
if (first_arg){
(first_arg = false);
continue;
}
if ((!(((file_name).has_value())))){
(file_name = arg);
continue;
}
if ((!(interpret_run))){
warnln(String("Extra unknown argument '{}', you can only pass one source file (was '{}')"),arg,file_name);
warnln(String("{}"),usage());
return (static_cast<i64>(1LL));
}
TRY((((interpreted_main_arguments).push(arg))));
}

}
}

if ((!(((file_name).has_value())))){
warnln(String("you must pass a source file"));
warnln(String("{}"),usage());
return (static_cast<i64>(1LL));
}
const NonnullRefPtr<utility::FilePath> file_path = TRY((utility::FilePath::make((file_name.value()))));
const String guessed_output_filename = ((binary_dir + String("/")) + TRY((((file_path)->basename(true)))));
const String output_filename = set_output_filename.value_or_lazy_evaluated([&] { return guessed_output_filename; });
JaktInternal::Array<error::JaktError> errors = (TRY((Array<error::JaktError>::create_with({}))));
NonnullRefPtr<compiler::Compiler> compiler = TRY((compiler::Compiler::create((TRY((Array<NonnullRefPtr<utility::FilePath>>::create_with({})))),(TRY((Dictionary<String, utility::FileId>::create_with_entries({})))),(TRY((Array<error::JaktError>::create_with({})))),JaktInternal::OptionalNone(),(TRY((Array<u8>::create_with({})))),lexer_debug,parser_debug,false,debug_print,extra_include_paths,json_errors,dump_type_hints,dump_try_hints,optimize,target_triple)));
TRY((((compiler)->load_prelude())));
const utility::FileId main_file_id = TRY((((compiler)->get_file_id_or_register(file_path))));
const bool file_is_set = ((compiler)->set_current_file(main_file_id));
if ((!(file_is_set))){
return (static_cast<i64>(1LL));
}
const JaktInternal::Array<lexer::Token> tokens = TRY((lexer::Lexer::lex(compiler)));
if (lexer_debug){
{
JaktInternal::ArrayIterator<lexer::Token> _magic = ((tokens).iterator());
for (;;){
JaktInternal::Optional<lexer::Token> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
lexer::Token token = (_magic_value.value());
{
outln(String("token: {}"),token);
}

}
}

}
const JaktInternal::Optional<FormatRange> format_range = TRY((parse_format_range(input_format_range,((((compiler)->current_file_contents)).size()))));
if ((!(((format_range).has_value())))){
warnln(String("invalid format range '{}', expected <start>(:<end>?)"),input_format_range);
return (static_cast<i64>(1LL));
}
if (format){
bool on_new_line = true;
{
formatter::Formatter _magic = TRY((formatter::Formatter::for_tokens(tokens,static_cast<size_t>(120ULL))));
for (;;){
JaktInternal::Optional<JaktInternal::Array<formatter::FormattedToken>> _magic_value = TRY((((_magic).next())));
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Array<formatter::FormattedToken> formatted_line = (_magic_value.value());
{
{
JaktInternal::ArrayIterator<formatter::FormattedToken> _magic = ((formatted_line).iterator());
for (;;){
JaktInternal::Optional<formatter::FormattedToken> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
formatter::FormattedToken formatted_token = (_magic_value.value());
{
if ((!(((((((formatted_token).token)).span())).is_in_offset_range((((format_range.value())).start),(((format_range.value())).end)))))){
continue;
}
{
JaktInternal::ArrayIterator<u8> _magic = ((((formatted_token).preceding_trivia)).iterator());
for (;;){
JaktInternal::Optional<u8> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
u8 byte = (_magic_value.value());
{
out(String("{:c}"),byte);
}

}
}

JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_AT_LOOP(([&]() -> JaktInternal::ExplicitValueOrControlFlow<void, ErrorOr<int>>{
auto&& __jakt_match_variant = ((formatted_token).token);
switch(__jakt_match_variant.index()) {
case 54: {
auto&& __jakt_match_value = __jakt_match_variant.template get<lexer::Token::Eol>();JaktInternal::Optional<String> const& comment = __jakt_match_value.comment;
{
if (((comment).has_value())){
u8 next_char = ' ';
if (((((comment.value())).length()) != static_cast<size_t>(0ULL))){
(next_char = (((comment.value())).byte_at(static_cast<size_t>(0ULL))));
}
const String space = JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_AT_LOOP_NESTED_MATCH(([&]() -> JaktInternal::ExplicitValueOrControlFlow<String,ErrorOr<int>>{
auto __jakt_enum_value = (next_char);
if (__jakt_enum_value == ' ') {
return JaktInternal::ExplicitValue(String(""));
}
else if (__jakt_enum_value == '\t') {
return JaktInternal::ExplicitValue(String(""));
}
else if (__jakt_enum_value == '/') {
return JaktInternal::ExplicitValue(String(""));
}
else {
return JaktInternal::ExplicitValue(String(" "));
}
}()))
;
const String lhs_space = JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_AT_LOOP_NESTED_MATCH(([&]() -> JaktInternal::ExplicitValueOrControlFlow<String,ErrorOr<int>>{
auto __jakt_enum_value = (on_new_line);
if (__jakt_enum_value == true) {
return JaktInternal::ExplicitValue(TRY((indent(((formatted_token).indent)))));
}
else {
return JaktInternal::ExplicitValue(String(" "));
}
}()))
;
out(String("{}//{}{}"),lhs_space,space,(comment.value()));
}
(on_new_line = true);
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
default: {
{
if (on_new_line){
out(TRY((indent(((formatted_token).indent)))));
}
out(String("{}"),TRY((((formatted_token).token_text()))));
(on_new_line = false);
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
}/*switch end*/
}()
));
{
JaktInternal::ArrayIterator<u8> _magic = ((((formatted_token).trailing_trivia)).iterator());
for (;;){
JaktInternal::Optional<u8> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
u8 byte = (_magic_value.value());
{
out(String("{:c}"),byte);
}

}
}

if (((((formatted_token).token)).index() == 54 /* Eol */)){
outln();
}
}

}
}

}

}
}

return (static_cast<i64>(0LL));
}
const parser::ParsedNamespace parsed_namespace = TRY((parser::Parser::parse(compiler,tokens)));
if (parser_debug){
outln(String("{:#}"),parsed_namespace);
}
const NonnullRefPtr<types::CheckedProgram> checked_program = TRY((typechecker::Typechecker::typecheck(compiler,parsed_namespace)));
if (interpret_run){
NonnullRefPtr<interpreter::Interpreter> interpreter = TRY((interpreter::Interpreter::create(compiler,checked_program,(TRY((Array<utility::Span>::create_with({})))))));
const types::ScopeId prelude_scope_id = types::ScopeId(types::ModuleId(static_cast<size_t>(0ULL)),static_cast<size_t>(0ULL));
JaktInternal::Optional<types::FunctionId> main_function_id = JaktInternal::OptionalNone();
{
JaktInternal::ArrayIterator<NonnullRefPtr<types::Module>> _magic = ((((checked_program)->modules)).iterator());
for (;;){
JaktInternal::Optional<NonnullRefPtr<types::Module>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
NonnullRefPtr<types::Module> module = (_magic_value.value());
{
{
JaktInternal::ArrayIterator<NonnullRefPtr<types::Scope>> _magic = ((((module)->scopes)).iterator());
for (;;){
JaktInternal::Optional<NonnullRefPtr<types::Scope>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
NonnullRefPtr<types::Scope> scope = (_magic_value.value());
{
if ((!(((((scope)->parent)).map([&](auto& _value) { return _value.equals(prelude_scope_id); })).value_or_lazy_evaluated([&] { return false; })))){
continue;
}
{
JaktInternal::DictionaryIterator<String,types::FunctionId> _magic = ((((scope)->functions)).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<String,types::FunctionId>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<String,types::FunctionId> function_ = (_magic_value.value());
{
if ((((function_).get<0>()) == String("main"))){
(main_function_id = ((function_).get<1>()));
break;
}
}

}
}

if (((main_function_id).has_value())){
break;
}
}

}
}

if (((main_function_id).has_value())){
break;
}
}

}
}

if ((!(((main_function_id).has_value())))){
warnln(String("Error: No main function found in program"));
return (static_cast<i64>(1LL));
}
const JaktInternal::Array<types::ResolvedNamespace> namespace_ = (TRY((Array<types::ResolvedNamespace>::create_with({}))));
const utility::Span call_span = utility::Span(utility::FileId(static_cast<size_t>(0ULL)),static_cast<size_t>(0ULL),static_cast<size_t>(0ULL));
const JaktInternal::Optional<types::CheckedParameter> first_main_param = ((((((checked_program)->get_function((main_function_id.value()))))->params)).first());
const JaktInternal::Array<types::Value> arguments = JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_RETURN_ONLY(([&]() -> JaktInternal::ExplicitValueOrControlFlow<JaktInternal::Array<types::Value>,ErrorOr<int>>{
auto __jakt_enum_value = (((first_main_param).has_value()));
if (__jakt_enum_value == true) {
return JaktInternal::ExplicitValue(({ Optional<JaktInternal::Array<types::Value>> __jakt_var_551; {
JaktInternal::Array<types::Value> passed_arguments = (TRY((Array<types::Value>::create_with({types::Value(TRY((types::ValueImpl::template create<typename types::ValueImpl::JaktString>((file_name.value())))),call_span)}))));
{
JaktInternal::ArrayIterator<String> _magic = ((interpreted_main_arguments).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String argument = (_magic_value.value());
{
TRY((((passed_arguments).push(types::Value(TRY((types::ValueImpl::template create<typename types::ValueImpl::JaktString>(argument))),call_span)))));
}

}
}

__jakt_var_551 = (TRY((Array<types::Value>::create_with({types::Value(TRY((types::ValueImpl::template create<typename types::ValueImpl::JaktArray>(passed_arguments,(((((first_main_param.value())).variable)).type_id)))),call_span)})))); goto __jakt_label_509;

}
__jakt_label_509:; __jakt_var_551.release_value(); }));
}
else {
return JaktInternal::ExplicitValue((TRY((Array<types::Value>::create_with({})))));
}
}()))
;
const interpreter::ExecutionResult main_result = TRY((((interpreter)->execute((main_function_id.value()),namespace_,JaktInternal::OptionalNone(),arguments,call_span,JaktInternal::OptionalNone()))));
JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_RETURN_ONLY(([&]() -> JaktInternal::ExplicitValueOrControlFlow<void, ErrorOr<int>>{
auto&& __jakt_match_variant = main_result;
switch(__jakt_match_variant.index()) {
case 0: {
auto&& __jakt_match_value = __jakt_match_variant.template get<typename interpreter::ExecutionResult::Return>();
types::Value const& x = __jakt_match_value.value;
return (JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_RETURN_ONLY(([&]() -> JaktInternal::ExplicitValueOrControlFlow<void, ErrorOr<int>>{
auto&& __jakt_match_variant = *((x).impl);
switch(__jakt_match_variant.index()) {
case 15: {
auto&& __jakt_match_value = __jakt_match_variant.template get<typename types::ValueImpl::CInt>();
int const& ret_val = __jakt_match_value.value;
{
return (ret_val);
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
case 9: {
auto&& __jakt_match_value = __jakt_match_variant.template get<typename types::ValueImpl::I64>();
i64 const& ret_val = __jakt_match_value.value;
{
return (ret_val);
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
case 0: {
auto&& __jakt_match_value = __jakt_match_variant.template get<typename types::ValueImpl::Void>();
{
return (static_cast<i64>(0LL));
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
default: {
{
warnln(String("Error: Main function must return an integer"));
return (static_cast<i64>(1LL));
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
}/*switch end*/
}()
))), JaktInternal::ExplicitValue<void>();
};/*case end*/
case 1: {
auto&& __jakt_match_value = __jakt_match_variant.template get<typename interpreter::ExecutionResult::Throw>();
types::Value const& x = __jakt_match_value.value;
{
warnln(String("Error: Main function threw: {}"),TRY((repl::serialize_ast_node(TRY((interpreter::value_to_checked_expression(x,interpreter)))))));
return (static_cast<i64>(1LL));
}
return JaktInternal::ExplicitValue<void>();
};/*case end*/
default: VERIFY_NOT_REACHED();}/*switch end*/
}()
));
}
if (((goto_def).has_value())){
const size_t index = (infallible_integer_cast<size_t>((((((goto_def.value())).to_uint()).value()))));
const utility::Span result = TRY((ide::find_definition_in_program(checked_program,utility::Span(utility::FileId(static_cast<size_t>(1ULL)),index,index))));
if ((((((result).file_id)).id) == static_cast<size_t>(1ULL))){
outln(String("{{\"start\": {}, \"end\": {}}}"),((result).start),((result).end));
}
else {
const JaktInternal::Optional<NonnullRefPtr<utility::FilePath>> file_path = TRY((((compiler)->get_file_path(((result).file_id)))));
outln(String("{{\"start\": {}, \"end\": {}, \"file\": \"{}\"}}"),((result).start),((result).end),TRY((utility::escape_for_quotes((((file_path.value()))->path)))));
}

return (static_cast<i64>(0LL));
}
if (((goto_type_def).has_value())){
const size_t index = (infallible_integer_cast<size_t>((((((goto_type_def.value())).to_uint()).value()))));
const utility::Span result = TRY((ide::find_type_definition_in_program(checked_program,utility::Span(utility::FileId(static_cast<size_t>(1ULL)),index,index))));
if ((((((result).file_id)).id) == static_cast<size_t>(1ULL))){
outln(String("{{\"start\": {}, \"end\": {}}}"),((result).start),((result).end));
}
else {
const JaktInternal::Optional<NonnullRefPtr<utility::FilePath>> file_path = TRY((((compiler)->get_file_path(((result).file_id)))));
outln(String("{{\"start\": {}, \"end\": {}, \"file\": \"{}\"}}"),((result).start),((result).end),TRY((utility::escape_for_quotes((((file_path.value()))->path)))));
}

return (static_cast<i64>(0LL));
}
if (((hover).has_value())){
const size_t index = (infallible_integer_cast<size_t>((((((hover.value())).to_uint()).value()))));
const JaktInternal::Optional<String> result = TRY((ide::find_typename_in_program(checked_program,utility::Span(utility::FileId(static_cast<size_t>(1ULL)),index,index))));
if (((result).has_value())){
outln(String("{{\"hover\": \"{}\"}}"),(result.value()));
}
return (static_cast<i64>(0LL));
}
if (((completions).has_value())){
const size_t index = (infallible_integer_cast<size_t>((((((completions.value())).to_uint()).value()))));
const JaktInternal::Array<String> result = TRY((ide::find_dot_completions(checked_program,utility::Span(utility::FileId(static_cast<size_t>(1ULL)),index,index))));
out(String("{{\"completions\": ["));
bool first = true;
{
JaktInternal::ArrayIterator<String> _magic = ((result).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String completion = (_magic_value.value());
{
if ((!(first))){
out(String(", "));
}
else {
(first = false);
}

out(String("\"{}\""),completion);
}

}
}

outln(String("]}}"));
return (static_cast<i64>(0LL));
}
if (typechecker_debug){
outln(String("{:#}"),checked_program);
}
TRY((((compiler)->print_errors())));
if ((!(((((compiler)->errors)).is_empty())))){
return (static_cast<i64>(1LL));
}
if (check_only){
return (static_cast<i64>(0LL));
}
const JaktInternal::Dictionary<String,JaktInternal::Tuple<String,String>> codegen_result = TRY((codegen::CodeGenerator::generate(compiler,checked_program,codegen_debug)));
StringBuilder depfile_builder = TRY((StringBuilder::create()));
if ((!(File::exists(binary_dir)))){
TRY((unknown_fs::make_directory(binary_dir)));
}
{
JaktInternal::DictionaryIterator<String,JaktInternal::Tuple<String,String>> _magic = ((codegen_result).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> file__contents_and_path__ = (_magic_value.value());
{
const JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> jakt__file__contents_and_path__ = file__contents_and_path__;
const String file = ((jakt__file__contents_and_path__).get<0>());
const JaktInternal::Tuple<String,String> contents_and_path = ((jakt__file__contents_and_path__).get<1>());

const JaktInternal::Tuple<String,String> contents_module_file_path_ = contents_and_path;
const String contents = ((contents_module_file_path_).get<0>());
const String module_file_path = ((contents_module_file_path_).get<1>());

const String path = ((binary_dir + String("/")) + file);
auto __jakt_var_553 = [&]() -> ErrorOr<void> { return TRY((write_to_file(contents,path))), ErrorOr<void>{}; }();
if (__jakt_var_553.is_error()) {auto error = __jakt_var_553.release_error();
{
warnln(String("Error: Could not write to file: {} ({})"),file,error);
return (static_cast<i64>(1LL));
}
}
;
if ((((generate_depfile).has_value()) && ((file).ends_with(String(".cpp"))))){
const String escaped = TRY((((file).replace(String(" "),String("\\ ")))));
const String escaped_module_file_path = TRY((((module_file_path).replace(String(" "),String("\\ ")))));
TRY((((depfile_builder).append_string(TRY((String::formatted(String("{} {}.h: {}"),escaped,TRY((((escaped).substring(static_cast<size_t>(0ULL),(JaktInternal::checked_sub<size_t>(((file).length()),static_cast<size_t>(4ULL))))))),escaped_module_file_path)))))));
TRY((((depfile_builder).append('\n'))));
}
}

}
}

if (((generate_depfile).has_value())){
auto __jakt_var_554 = [&]() -> ErrorOr<void> {{
TRY((write_to_file(TRY((((depfile_builder).to_string()))),(generate_depfile.value()))));
}

;return {};}();
if (__jakt_var_554.is_error()) {auto error = __jakt_var_554.release_error();{
warnln(String("Error: Could not write to file list ({})"),error);
return (static_cast<i64>(1LL));
}
};
}
if ((build_executable || run_executable)){
JaktInternal::Set<size_t> ids = (TRY((Set<size_t>::create_with_values({}))));
JaktInternal::Array<String> linked_files = (TRY((Array<String>::create_with({}))));
ParallelExecutionPool pool = TRY((ParallelExecutionPool::create(max_concurrent)));
i64 total_jobs = static_cast<i64>(0LL);
{
JaktInternal::DictionaryIterator<String,JaktInternal::Tuple<String,String>> _magic = ((codegen_result).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> file_name_____ = (_magic_value.value());
{
const JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> jakt__file_name_____ = file_name_____;
const String file_name = ((jakt__file_name_____).get<0>());
const JaktInternal::Tuple<String,String> _ = ((jakt__file_name_____).get<1>());

if (((file_name).ends_with(String(".h")))){
continue;
}
({auto& _jakt_ref = total_jobs;_jakt_ref = JaktInternal::checked_add<i64>(_jakt_ref, static_cast<i64>(1LL));});
}

}
}

{
JaktInternal::DictionaryIterator<String,JaktInternal::Tuple<String,String>> _magic = ((codegen_result).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> file_name_____ = (_magic_value.value());
{
const JaktInternal::Tuple<String,JaktInternal::Tuple<String,String>> jakt__file_name_____ = file_name_____;
const String file_name = ((jakt__file_name_____).get<0>());
const JaktInternal::Tuple<String,String> _ = ((jakt__file_name_____).get<1>());

if (((file_name).ends_with(String(".h")))){
continue;
}
{
JaktInternal::DictionaryIterator<size_t,unknown_process::ExitPollResult> _magic = ((((pool).completed)).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<size_t,unknown_process::ExitPollResult>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> id__exit_result__ = (_magic_value.value());
{
const JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> jakt__id__exit_result__ = id__exit_result__;
const size_t id = ((jakt__id__exit_result__).get<0>());
const unknown_process::ExitPollResult exit_result = ((jakt__id__exit_result__).get<1>());

if ((((exit_result).exit_code) != static_cast<i32>(0))){
warnln(String("Error: Compilation failed"));
TRY((((pool).kill_all())));
return (static_cast<i64>(1LL));
}
}

}
}

const NonnullRefPtr<utility::FilePath> file_path = TRY((utility::FilePath::make(file_name)));
const String basename_without_extension = TRY((((TRY((((file_path)->basename(false))))).substring(static_cast<size_t>(0ULL),(JaktInternal::checked_sub<size_t>(((TRY((((file_path)->basename(false))))).length()),static_cast<size_t>(4ULL)))))));
const String built_object = (((binary_dir + String("/")) + basename_without_extension) + String(".o"));
TRY((((linked_files).push(built_object))));
const size_t id = TRY((run_compiler(((pool)),cxx_compiler_path,((binary_dir + String("/")) + file_name),built_object,runtime_path,extra_include_paths,(TRY((Array<String>::create_with({})))),(TRY((Array<String>::create_with({})))),optimize,(TRY((Array<String>::create_with({String("-c")})))))));
TRY((((ids).add(id))));
warnln(String("{:c}[2LBuilding: {}/{}"),static_cast<i64>(27LL),((ids).size()),total_jobs);
}

}
}

TRY((((pool).wait_for_all_jobs_to_complete())));
{
JaktInternal::DictionaryIterator<size_t,unknown_process::ExitPollResult> _magic = ((((pool).completed)).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<size_t,unknown_process::ExitPollResult>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> id__exit_result__ = (_magic_value.value());
{
const JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> jakt__id__exit_result__ = id__exit_result__;
const size_t id = ((jakt__id__exit_result__).get<0>());
const unknown_process::ExitPollResult exit_result = ((jakt__id__exit_result__).get<1>());

if ((((exit_result).exit_code) != static_cast<i32>(0))){
warnln(String("Error: Compilation failed"));
return (static_cast<i64>(1LL));
}
}

}
}

JaktInternal::Array<String> linker_args = (TRY((Array<String>::create_with({}))));
if (((link_archive).has_value())){
(linker_args = (TRY((Array<String>::create_with({archiver_path.value_or_lazy_evaluated([&] { return String("ar"); }), String("cr"), (link_archive.value())})))));
{
JaktInternal::ArrayIterator<String> _magic = ((linked_files).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String arg = (_magic_value.value());
{
TRY((((linker_args).push(arg))));
}

}
}

}
else {
(linker_args = (TRY((Array<String>::create_with({cxx_compiler_path, String("-o"), output_filename})))));
{
JaktInternal::ArrayIterator<String> _magic = ((linked_files).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String arg = (_magic_value.value());
{
TRY((((linker_args).push(arg))));
}

}
}

const String extension = JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_RETURN_ONLY(([&]() -> JaktInternal::ExplicitValueOrControlFlow<String,ErrorOr<int>>{
auto __jakt_enum_value = (((TRY((os::Target::active()))).os));
if (__jakt_enum_value == String("windows")) {
return JaktInternal::ExplicitValue(String("lib"));
}
else {
return JaktInternal::ExplicitValue(String("a"));
}
}()))
;
TRY((((linker_args).push(TRY((String::formatted(String("{}/libjakt_runtime.{}"),runtime_library_path,extension)))))));
TRY((((linker_args).push(TRY((String::formatted(String("{}/libjakt_main.{}"),runtime_library_path,extension)))))));
{
JaktInternal::ArrayIterator<String> _magic = ((extra_lib_paths).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String path = (_magic_value.value());
{
TRY((((linker_args).push(String("-L")))));
TRY((((linker_args).push(path))));
}

}
}

{
JaktInternal::ArrayIterator<String> _magic = ((extra_link_libs).iterator());
for (;;){
JaktInternal::Optional<String> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
String lib = (_magic_value.value());
{
TRY((((linker_args).push(String("-l")))));
TRY((((linker_args).push(lib))));
}

}
}

}

const size_t id = TRY((((pool).run(linker_args))));
TRY((((pool).wait_for_all_jobs_to_complete())));
const unknown_process::ExitPollResult result = (((pool).status(id)).value());
if ((((result).exit_code) != static_cast<i32>(0))){
return (((result).exit_code));
}
}
if (run_executable){
return (system(((output_filename).c_string())));
}
}
return 0;
}

String help() {
{
String output = String("Flags:\n");
(output += String("  -h,--help\t\t\t\tPrint this help and exit.\n"));
(output += String("  -O\t\t\t\t\tBuild an optimized executable.\n"));
(output += String("  -dl\t\t\t\t\tPrint debug info for the lexer.\n"));
(output += String("  -dp\t\t\t\t\tPrint debug info for the parser.\n"));
(output += String("  -dt\t\t\t\t\tPrint debug info for the typechecker.\n"));
(output += String("  -S\t\t\t\t\tOnly output source (do not build).\n"));
(output += String("  -T,--target-triple TARGET\t\t\t\t\tSpecify the target triple used for the build, defaults to native.\n"));
(output += String("  --runtime-library-path PATH\t\t\t\t\tSpecify the path to the runtime library, defaults to 'build/runtime'.\n"));
(output += String("  -J,--jobs NUMBER\t\t\t\t\tSpecify the number of jobs to run in parallel, defaults to 2.\n"));
(output += String("  -cr, --compile-run\t\t\tBuild and run an executable file.\n"));
(output += String("  -r, --run\t\t\t\tRun the given file without compiling it (all positional arguments after the file name will be passed to main).\n"));
(output += String("  -d\t\t\t\t\tInsert debug statement spans in generated C++ code.\n"));
(output += String("  --debug-print\t\t\t\tOutput debug print.\n"));
(output += String("  -p --prettify-cpp-source\t\tRun emitted C++ source through clang-format.\n"));
(output += String("  -S,--emit-cpp-source-only\t\tWrite the C++ source to file, even when not building/\n"));
(output += String("  -c,--check-only\t\t\tOnly check the code for errors.\n"));
(output += String("  -j,--json-errors\t\t\tEmit machine-readable (JSON) errors.\n"));
(output += String("  -H,--type-hints\t\t\tEmit machine-readable type hints (for IDE integration).\n"));
(output += String("  --try-hints\t\t\t\tEmit machine-readable try hints (for IDE integration).\n"));
(output += String("  --repl\t\t\t\tStart a Read-Eval-Print loop session.\n"));
(output += String("\nOptions:\n"));
(output += String("  -F,--clang-format-path PATH\t\tPath to clang-format executable.\n\t\t\t\t\tDefaults to clang-format\n"));
(output += String("  -D,--dot-clang-format-path PATH\tPath to the .clang-format file to use.\n\t\t\t\t\tDefaults to none, invoking clangs default .clang-format file handling.\n"));
(output += String("  -R,--runtime-path PATH\t\tPath of the Jakt runtime headers.\n\t\t\t\t\tDefaults to $PWD/runtime.\n"));
(output += String("  -B,--binary-dir PATH\t\t\tOutput directory for compiled files.\n\t\t\t\t\tDefaults to $PWD/build.\n"));
(output += String("  -C,--cxx-compiler-path PATH\t\tPath of the C++ compiler to use when compiling the generated sources.\n\t\t\t\t\tDefaults to clang++.\n"));
(output += String("  -I PATH\t\t\t\tAdd PATH to compiler's include list. Can be specified multiple times.\n"));
(output += String("  -L PATH\t\t\t\tAdd PATH to linker's search list. Can be specified multiple times.\n"));
(output += String("  -l,--link-with LIB\t\t\tLink executable with LIB. Can be specified multiple times.\n"));
(output += String("  -o,--output-filename FILE\t\tName of the output binary.\n\t\t\t\t\tDefaults to the input-filename without the extension.\n"));
(output += String("  -g,--goto-def INDEX\t\t\tReturn the span for the definition at index.\n"));
(output += String("  -t,--goto-type-def INDEX\t\tReturn the span for the type definition at index.\n"));
(output += String("  -e,--hover INDEX\t\t\tReturn the type of element at index.\n"));
(output += String("  -m,--completions INDEX\t\tReturn dot completions at index.\n"));
return (output);
}
}

String usage() {
{
return (String("usage: jakt [-h] [OPTIONS] <filename>"));
}
}

ErrorOr<JaktInternal::Optional<FormatRange>> parse_format_range(const String range,const size_t input_file_length) {
{
const JaktInternal::Array<String> parts = TRY((((range).split(':'))));
if (((parts).is_empty())){
return (FormatRange(static_cast<size_t>(0ULL),input_file_length));
}
const JaktInternal::Optional<u32> start_input = ((((parts)[static_cast<i64>(0LL)])).to_uint());
if ((!(((start_input).has_value())))){
return (JaktInternal::OptionalNone());
}
const size_t start = (infallible_integer_cast<size_t>(((start_input.value()))));
const size_t end = JAKT_RESOLVE_EXPLICIT_VALUE_OR_CONTROL_FLOW_RETURN_ONLY(([&]() -> JaktInternal::ExplicitValueOrControlFlow<size_t,ErrorOr<JaktInternal::Optional<FormatRange>>>{
auto __jakt_enum_value = (((parts).size()));
if (__jakt_enum_value == static_cast<size_t>(1ULL)) {
return JaktInternal::ExplicitValue(input_file_length);
}
else if (__jakt_enum_value == static_cast<size_t>(2ULL)) {
return JaktInternal::ExplicitValue(({ Optional<size_t> __jakt_var_555; {
const JaktInternal::Optional<u32> end_input = ((((parts)[static_cast<i64>(1LL)])).to_uint());
if ((!(((end_input).has_value())))){
return (JaktInternal::OptionalNone());
}
__jakt_var_555 = (infallible_integer_cast<size_t>(((end_input.value())))); goto __jakt_label_511;

}
__jakt_label_511:; __jakt_var_555.release_value(); }));
}
else {
{
return (JaktInternal::OptionalNone());
}
}
}()))
;
return (FormatRange(start,end));
}
}

ErrorOr<String> ParallelExecutionPool::debug_description() const { auto builder = MUST(StringBuilder::create());TRY(builder.append("ParallelExecutionPool("));{
JaktInternal::PrettyPrint::ScopedLevelIncrease increase_indent {};
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("pids: "));TRY(builder.appendff("{}, ", pids));
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("completed: "));TRY(builder.appendff("{}, ", completed));
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("pid_index: "));TRY(builder.appendff("{}, ", pid_index));
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("max_concurrent: "));TRY(builder.appendff("{}", max_concurrent));
}
TRY(builder.append(")"));return builder.to_string(); }
ErrorOr<ParallelExecutionPool> ParallelExecutionPool::create(const size_t max_concurrent) {
{
return (ParallelExecutionPool((TRY((Dictionary<size_t, unknown_process::Process>::create_with_entries({})))),(TRY((Dictionary<size_t, unknown_process::ExitPollResult>::create_with_entries({})))),static_cast<size_t>(0ULL),max_concurrent));
}
}

JaktInternal::Optional<unknown_process::ExitPollResult> ParallelExecutionPool::status(const size_t id) const {
{
if (((((*this).completed)).contains(id))){
return (((((*this).completed))[id]));
}
return (JaktInternal::OptionalNone());
}
}

ParallelExecutionPool::ParallelExecutionPool(JaktInternal::Dictionary<size_t,unknown_process::Process> a_pids, JaktInternal::Dictionary<size_t,unknown_process::ExitPollResult> a_completed, size_t a_pid_index, size_t a_max_concurrent) :pids(a_pids), completed(a_completed), pid_index(a_pid_index), max_concurrent(a_max_concurrent){}

ErrorOr<void> ParallelExecutionPool::wait_for_any_job_to_complete() {
{
const unknown_process::ExitPollResult finished_status = TRY((unknown_process::wait_for_some_set_of_processes_that_at_least_includes(((((*this).pids))))));
JaktInternal::Dictionary<size_t,unknown_process::ExitPollResult> pids_to_remove = (TRY((Dictionary<size_t, unknown_process::ExitPollResult>::create_with_entries({}))));
{
JaktInternal::DictionaryIterator<size_t,unknown_process::Process> _magic = ((((*this).pids)).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<size_t,unknown_process::Process>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<size_t,unknown_process::Process> index__process__ = (_magic_value.value());
{
const JaktInternal::Tuple<size_t,unknown_process::Process> jakt__index__process__ = index__process__;
const size_t index = ((jakt__index__process__).get<0>());
const unknown_process::Process process = ((jakt__index__process__).get<1>());

const JaktInternal::Optional<unknown_process::ExitPollResult> status = ({ Optional<JaktInternal::Optional<unknown_process::ExitPollResult>> __jakt_var_556;
auto __jakt_var_557 = [&]() -> ErrorOr<JaktInternal::Optional<unknown_process::ExitPollResult>> { return TRY((unknown_process::poll_process_exit(((process))))); }();
if (__jakt_var_557.is_error()) {{
TRY((((pids_to_remove).set(index,finished_status))));
continue;
}
} else {__jakt_var_556 = __jakt_var_557.release_value();
}
__jakt_var_556.release_value(); });
if (((status).has_value())){
TRY((((pids_to_remove).set(index,(status.value())))));
}
}

}
}

{
JaktInternal::DictionaryIterator<size_t,unknown_process::ExitPollResult> _magic = ((pids_to_remove).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<size_t,unknown_process::ExitPollResult>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> index__status__ = (_magic_value.value());
{
const JaktInternal::Tuple<size_t,unknown_process::ExitPollResult> jakt__index__status__ = index__status__;
const size_t index = ((jakt__index__status__).get<0>());
const unknown_process::ExitPollResult status = ((jakt__index__status__).get<1>());

((((*this).pids)).remove(index));
TRY((((((*this).completed)).set(index,status))));
}

}
}

}
return {};
}

ErrorOr<size_t> ParallelExecutionPool::run(const JaktInternal::Array<String> args) {
{
if ((((((*this).pids)).size()) >= ((*this).max_concurrent))){
TRY((((*this).wait_for_any_job_to_complete())));
}
const unknown_process::Process process = TRY((unknown_process::start_background_process(args)));
const size_t id = ((((*this).pid_index)++));
TRY((((((*this).pids)).set(id,process))));
return (id);
}
}

ErrorOr<void> ParallelExecutionPool::kill_all() {
{
{
JaktInternal::DictionaryIterator<size_t,unknown_process::Process> _magic = ((((*this).pids)).iterator());
for (;;){
JaktInternal::Optional<JaktInternal::Tuple<size_t,unknown_process::Process>> _magic_value = ((_magic).next());
if ((!(((_magic_value).has_value())))){
break;
}
JaktInternal::Tuple<size_t,unknown_process::Process> ___process__ = (_magic_value.value());
{
const JaktInternal::Tuple<size_t,unknown_process::Process> jakt_____process__ = ___process__;
const size_t _ = ((jakt_____process__).get<0>());
const unknown_process::Process process = ((jakt_____process__).get<1>());

TRY((unknown_process::forcefully_kill_process(((process)))));
}

}
}

}
return {};
}

ErrorOr<void> ParallelExecutionPool::wait_for_all_jobs_to_complete() {
{
while ((!(((((*this).pids)).is_empty())))){
TRY((((*this).wait_for_any_job_to_complete())));
}
}
return {};
}

ErrorOr<String> FormatRange::debug_description() const { auto builder = MUST(StringBuilder::create());TRY(builder.append("FormatRange("));{
JaktInternal::PrettyPrint::ScopedLevelIncrease increase_indent {};
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("start: "));TRY(builder.appendff("{}, ", start));
TRY(JaktInternal::PrettyPrint::output_indentation(builder));TRY(builder.append("end: "));TRY(builder.appendff("{}", end));
}
TRY(builder.append(")"));return builder.to_string(); }
FormatRange::FormatRange(size_t a_start, size_t a_end) :start(a_start), end(a_end){}

} // namespace Jakt
