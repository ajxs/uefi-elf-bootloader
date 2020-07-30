# Coding Styleguide


**Rule:** Use snake case for variables.

**Rationale:** Improves ease of reading variable names quickly.

**Rule:** Observe `const` correctness at all times.

**Rule:** Provide initial values for declared variables where possible.

**Rule:** Only create a single variable per line.

**Rationale:** Resolves ambiguity of asterix position when declaring multiple pointer variables on a single line. Ensures a predictable standard for documentation of variables.

**Rule:** Never return in-band error indicators. Instead use either booleans to indicate the success of the function, or better yet use dedicated enumerated types to return detailed status information. Instead of returning a value directly, use a pointer to return the value instead.
**Rationale:** In-band error indicators returned from functions are prone to misinterpretation and are inflexible. Using dedicated types allows for the returning of detailed statuses if needed.
**Example:**
```c
// NO.
int32_t read_number_from_port()
{
	if(!port_ready()) {
		return -1;
	}

	return (int32_t)*port_ptr;
}

// YES.
bool read_number_from_port(uint32_t* output)
{
	if(!port_ready()) {
		return false;
	}

	*output = (uint32_t)*port_ptr;

	return true;
}

// BETTER.
typedef enum {
	STATUS_SUCCESS,
	STATUS_FAILURE,
	STATUS_BUSY
} Program_Status;

Program_Status read_number_from_port(uint32_t* output)
{
	if(!port_ready()) {
		return STATUS_BUSY;
	}

	*output = (uint32_t)*port_ptr;

	return STATUS_SUCCESS;
}
```

**Rule:** When possible, declare any required variables at the top of each code block. Exception being when this would make the code less readable.

**Rationale:** This makes it easier for another developer to gauge what variables are used in a given area of the code. As well as providing for consistent documentation.

**Rule:** Document each variable above its declaration.

**Rationale:** Provides a useful introduction to the use of each variable.

**Example:**
```c
/** The file being loaded by the application. */
FILE* file;
```

**Rule:** Use descriptive names with minimal use of abbreviations. When abbreviating names, use standard common abbreviations only. Such as:
- `dir` = Directory
- `idx` = Index
- `n_` = Number of ( prefix )
- `_t` = Type of ( suffix )
- `_len` = Length of ( suffix )
- `_addr` = Address of ( suffix )
- `_ptr` = Pointer to ( suffix )

**Exception:** Conventional usage of simple identifiers (i, x, y, p, etc.) in small scopes and for iterator variables generally leads to cleaner code.

**Rationale:** Eases the burden of understanding a variable's usage. Together with declaring each variable at the top of the scope block, this makes understanding the flow of data much easier.

**Example:**
```c
// YES.
uint8_t n_dir_entries = 0;
uint32_t entry_name_len = 255;
uintptr_t uart_addr = 0x800000C0; 
```

**Rule:** For qualifying a type as a pointer, attach the asterix to the type, not the variable name.

**Rationale:** This allows for a consistent style when casting `void` data into structs.

**Example:**
```c
// YES
FILE* file;
// NO
FILE *file;
```

**Rule:** Place comments above the relevant line of code where possible. If placing a comment on the same line that it documents, use space characters to align the comment, not TABs.

**Rule:** Use two line breaks after the end of a function body before the next line of code.

**Rule:** Use Doxygen-style inline documentation on all function declarations. Apply documentation to the declaration in the header files.
**Example:**
```c
/**
 * @brief brief description.
 *
 * Longer description
 * @param thing_ptr A pointer to the thing.
 * @return The status of the program.
 * @warning This function modifies the sections.
 */
Program_Status do_the_thing(Pointer_Type* thing_ptr);

```

**Rule:** In function declarations, place each parameter on a new line and the brace on the following line.

**Example:**
```c
/** retro_encabulate */
Program_Status retro_encabulate(uint8_t number,
  const char *string,
  bool flag)
{
  // ...
}
```

**Rule:** Use a standalone variable for a conditional if it is over 100 chars in length.

**Rationale:** This ensures that conditional branching logic is legible.

**Example:**
```c
// NO
if((something_long_and_complicated == (thing->bit_field & LONG_DESCRIPTIVE_BIT_MASK)) && 
	(another_sentinel_var == 16)) { 
		//...
}

// YES
/** Condition deciding whether to branch. */
bool shouldEncabulate = (something_long_and_complicated == 
	(thing->bit_field & LONG_DESCRIPTIVE_BIT_MASK)) && (another_sentinel_var == 16);

if(shouldEncabulate) {
	//...
}
```
