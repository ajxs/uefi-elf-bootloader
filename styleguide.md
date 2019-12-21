
# Coding Styleguide

  
**Rule:** Only create a single variable per line.
**Rationale:** Resolves ambiguity of asterix position when declaring multiple pointer variables on a single line.

**Rule:** When possible, declare any required variables at the top of each code block. Exception being when this would make the code less readable.
**Rationale:** This makes it easier for another developer to gauge what variables are used in a given area of the code. As well as providing for consistent documentation.

**Rule:** Document each variable above its declaration.
**Rationale:** Provides a useful introduction to the use of each variable.
**Example:** 
```c
/** The file being loaded by the application. */
EFI_FILE* file;
```

**Rule:** For qualifying a type as a pointer, attach the asterix to the type, not the variable name.
**Rationale:** This allows for a consistent style when casting `void` data into structs.
**Example:** 
```c
// YES
EFI_FILE* file;
// NO
EFI_FILE *file;
```

**Rule:** No in-band error checking.
**Rationale:** Avoids any potential ambiguity when processing the results of a function, as well as allowing for the returning of detailed error statuses if needed.
**Example:** 
```c
// YES
/** Allocates a buffer, returning the status of the operation. */
EFI_STATUS encabulate(char** buffer_to_allocate);

// NO
/** Allocates a buffer, assume error if NULL returned. */
char* encabulate();
```

