/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Option Parsing Definitions                                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

/////////
// Prefixes

#ifdef PREFIX
#define COMMA ,
PREFIX(prefix_0, {llvm::StringLiteral("")})
PREFIX(prefix_1, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
PREFIX(prefix_3, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_4, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_2, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
PREFIX(prefix_5, {llvm::StringLiteral("/") COMMA llvm::StringLiteral("")})
PREFIX(prefix_6, {llvm::StringLiteral("/") COMMA llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
#undef COMMA
#endif // PREFIX

/////////
// Prefix Union

#ifdef PREFIX_UNION
#define COMMA ,
PREFIX_UNION({
llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("/") COMMA llvm::StringLiteral("")})
#undef COMMA
#endif // PREFIX_UNION

/////////
// ValuesCode

#ifdef OPTTABLE_VALUES_CODE
#endif
/////////
// Groups

#ifdef OPTION
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "my group", my_group, Group, INVALID, INVALID, nullptr, 0, 0, 0, nullptr, nullptr, nullptr)

//////////
// Options

OPTION(prefix_0, "<input>", INPUT, Input, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, "<unknown>", UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-AB", AB, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-A", A, Flag, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The A option", nullptr, nullptr)
OPTION(prefix_2, "--blarn", Blarn, Flag, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The blarn option", nullptr, nullptr)
OPTION(prefix_3, "-blorp", Blorp, Flag, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The blorp option", nullptr, nullptr)
OPTION(prefix_4, "--blurmp=", Blurmpq_eq, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, "--blurmp", Blurmpq, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-B", B, Joined, INVALID, INVALID, nullptr, OptFlag2, DefaultVis, 0,
       "The B option", "B", nullptr)
OPTION(prefix_3, "-C=", Ceq, Joined, INVALID, C, nullptr, OptFlag1, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_5, "/cramb:", Cramb, Joined, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The cramb option", "CRAMB", nullptr)
OPTION(prefix_1, "-C", C, Separate, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The C option", "C", nullptr)
OPTION(prefix_6, "/C", SLASH_C, Separate, INVALID, INVALID, nullptr, OptFlag3, DefaultVis, 0,
       "The C option", "C", nullptr)
OPTION(prefix_1, "-doopf1", Doopf1, Flag, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The doopf1 option", nullptr, nullptr)
OPTION(prefix_1, "-doopf2", Doopf2, Flag, INVALID, INVALID, nullptr, OptFlag2, DefaultVis, 0,
       "The doopf2 option", nullptr, nullptr)
OPTION(prefix_1, "-D", D, CommaJoined, INVALID, INVALID, nullptr, 0, DefaultVis, 0,
       "The D option", "D", nullptr)
OPTION(prefix_4, "--ermgh", Ermgh, Joined, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The ermgh option", "ERMGH", nullptr)
OPTION(prefix_1, "-E", E, MultiArg, INVALID, INVALID, nullptr, OptFlag1 | OptFlag2, DefaultVis, 2, nullptr, nullptr, nullptr)
OPTION(prefix_4, "--fjormp", Fjormp, Flag, INVALID, INVALID, nullptr, OptFlag1, DefaultVis, 0,
       "The fjormp option", nullptr, nullptr)
OPTION(prefix_1, "-F", F, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, DefaultVis, 0,
       "The F option", "F", nullptr)
OPTION(prefix_4, "--glorrmp=", Glorrmp_eq, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-G", G, JoinedAndSeparate, INVALID, INVALID, nullptr, 0, DefaultVis, 0,
       "The G option", "G", nullptr)
OPTION(prefix_1, "-H", H, Flag, INVALID, INVALID, nullptr, HelpHidden, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-I", I, Flag, my_group, H, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-Joo", Joo, Flag, INVALID, B, "bar\0", 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-J", J, Flag, INVALID, B, "foo\0", 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-K", K, Flag, INVALID, B, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-marshalled-flag-a", marshalled_flag_a, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-marshalled-flag-b", marshalled_flag_b, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-marshalled-flag-c", marshalled_flag_c, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-marshalled-flag-d", marshalled_flag_d, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-Q", Q, Flag, INVALID, INVALID, nullptr, 0, SubtoolVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-R", R, Flag, INVALID, INVALID, nullptr, 0, DefaultVis | SubtoolVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-slurpjoined", SlurpJoined, RemainingArgsJoined, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-slurp", Slurp, RemainingArgs, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, "-xyzzy1", Xyzzy1, Flag, INVALID, INVALID, nullptr, 0, SubtoolVis, 0,
       "The xyzzy1 option", nullptr, nullptr)
OPTION(prefix_1, "-xyzzy2", Xyzzy2, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0,
       "The xyzzy2 option", nullptr, nullptr)
#endif // OPTION
#ifdef OPTION_WITH_MARSHALLING
OPTION_WITH_MARSHALLING(prefix_1, "-marshalled-flag-d", marshalled_flag_d, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr, true, 0, X->MarshalledFlagD, false, false, false, normalizeSimpleFlag, denormalizeSimpleFlag, mergeForwardValue, extractForwardValue, -1)
#endif // OPTION_WITH_MARSHALLING
#ifdef OPTION_WITH_MARSHALLING
OPTION_WITH_MARSHALLING(prefix_1, "-marshalled-flag-c", marshalled_flag_c, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr, true, 0, X->MarshalledFlagC, false, false || X->MarshalledFlagD, true, normalizeSimpleFlag, denormalizeSimpleFlag, mergeForwardValue, extractForwardValue, -1)
#endif // OPTION_WITH_MARSHALLING
#ifdef OPTION_WITH_MARSHALLING
OPTION_WITH_MARSHALLING(prefix_1, "-marshalled-flag-b", marshalled_flag_b, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr, true, 0, X->MarshalledFlagB, false, false || X->MarshalledFlagD, true, normalizeSimpleFlag, denormalizeSimpleFlag, mergeForwardValue, extractForwardValue, -1)
#endif // OPTION_WITH_MARSHALLING
#ifdef OPTION_WITH_MARSHALLING
OPTION_WITH_MARSHALLING(prefix_1, "-marshalled-flag-a", marshalled_flag_a, Flag, INVALID, INVALID, nullptr, 0, DefaultVis, 0, nullptr, nullptr, nullptr, true, 0, X->MarshalledFlagA, false, false || X->MarshalledFlagC || X->MarshalledFlagB, true, normalizeSimpleFlag, denormalizeSimpleFlag, mergeForwardValue, extractForwardValue, -1)
#endif // OPTION_WITH_MARSHALLING

#ifdef SIMPLE_ENUM_VALUE_TABLE

struct SimpleEnumValue {
  const char *Name;
  unsigned Value;
};

struct SimpleEnumValueTable {
  const SimpleEnumValue *Table;
  unsigned Size;
};
static const SimpleEnumValueTable SimpleEnumValueTables[] = {};
static const unsigned SimpleEnumValueTablesSize = std::size(SimpleEnumValueTables);
#endif // SIMPLE_ENUM_VALUE_TABLE


