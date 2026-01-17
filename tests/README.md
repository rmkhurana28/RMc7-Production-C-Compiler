# RMc7 Compiler Test Suite

**Author:** rm129  
**Purpose:** Comprehensive testing infrastructure for RMc7 Level-7 Production C Compiler

## Directory Structure

```
tests/
├── unit/                      # Unit tests for individual components
│   ├── test_lexer_*.c        # Lexer tokenization tests
│   ├── test_declarator_*.c   # Declarator parsing tests
│   ├── test_type_*.c         # Type system tests
│   └── test_*.c              # Other unit tests
├── expected_outputs/          # Expected output files for comparison
├── test_outputs/              # Generated outputs (created by test runner)
├── test_runner.sh            # Automated test execution script
└── README.md                 # This file
```

## Test Categories

### Phase 1 - Current Tests (Unit Tests)

#### Lexer Tests
- `test_lexer_basic.c` - Basic keywords and identifiers
- `test_lexer_operators.c` - All C operators

#### Declarator Tests
- `test_declarator_simple_pointers.c` - Single/double/triple pointers
- `test_declarator_complex_pointers.c` - Parenthesized pointer patterns (Session 19 fix)
- `test_declarator_arrays.c` - Array declarators
- `test_declarator_mixed.c` - Mixed pointers and arrays
- `test_declarator_first_var_stars.c` - First variable star application (Session 19)

#### Type System Tests
- `test_type_modifiers.c` - Sign and size modifiers
- `test_type_qualifiers.c` - const, volatile, restrict
- `test_void_declarations.c` - Special void type rules
- `test_struct_union_enum.c` - Type registry entries

### Phase 2 - Future Tests (To be added)
- Expression parsing tests
- Statement parsing tests
- Function definition tests
- Full program integration tests

## Running Tests

### Run All Tests
```bash
cd /home/rm_khurana129/Documents/RMc7
./tests/test_runner.sh all
```

### Run Specific Category
```bash
./tests/test_runner.sh lexer        # Lexer tests only
./tests/test_runner.sh declarator   # Declarator tests only
```

### Run Single Test
```bash
./rmc7 tests/unit/test_declarator_complex_pointers.c
```

## Creating Expected Outputs

When adding new tests, generate expected outputs:
```bash
./rmc7 tests/unit/test_name.c > tests/expected_outputs/test_name.expected
```

Review the output to ensure it's correct, then commit it as the expected result.

## Test Coverage

### Current Coverage (Phase 1)
- ✅ Lexer: 100% (all token types)
- ✅ Declarator parsing: ~95% (pending AST for arrays/functions)
- ✅ Type validation: ~90% (basic rules implemented)
- ⏳ Expression parsing: 0% (not started)
- ⏳ Statement parsing: 0% (not started)

### Known Limitations
- Array index expressions not evaluated (AST pending)
- Function parameters not parsed (AST pending)
- Multiple declarations (comma-separated) need deep copy implementation

## Notes

- Each test file starts with `//rm129` comment
- Tests are designed to be independent and focused
- Complex cases test Session 19 fix for deferred star application
- Test runner provides colored output for easy result scanning
