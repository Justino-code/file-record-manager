File Record Manager

A file-based record management system developed in C as a practical project for studying and applying programming and software engineering concepts.

The project starts as a book catalog stored in a binary file and will be progressively evolved as new concepts are learned.

Objective

The main goal is not only to build a catalog system, but to use the project as a practical environment for consolidating C programming knowledge and tracking the evolution of the code across different versions.

The concepts initially covered include:

- Structures ("struct")
- Functions
- Strings and arrays
- Basic pointers
- File handling
- Binary files
- "fread" and "fwrite"
- File positioning with "fseek"
- Record searching and updating
- Logical record deletion
- Input handling
- Code refactoring
- Version control

Current State

The initial version implements a book catalog with the following operations:

- Add
- List
- Search
- Update
- Remove

Records are stored in a binary file.

Versioning

The project uses version branches to separate stable releases from ongoing development.

The versioning convention is:

- "v1" — initial functional version
- "v1.x" — incremental improvements, refactoring, bug fixes, and other changes that do not represent a major evolution of the project
- "v2" — major evolution introducing a new set of concepts or significant changes
- "v2.x" — incremental improvements and refinements to version 2
- "v3" — next major evolution
- And so on.

The "main" branch always represents the latest stable version.

New versions are developed in their respective branches and merged into "main" once they are considered functional and stable.

Project Purpose

This project is primarily educational. The system will evolve incrementally as new concepts are studied and applied.

The initial domain is book management, but the project's architecture and evolution are not necessarily limited to this domain.

License

This project is licensed under the "MIT License" (LICENSE).
