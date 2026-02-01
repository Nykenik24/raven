# Program
> [!WARNING] Under revision.
> This file is under revision, as it might contain information that is not accurate.

```ts
interface Program {
	body: Declaration[];
}
```

---

# Declarations

```ts
type Declaration =
	| VariableDecl
	| FunctionDecl
	| StructureDecl
	| EnumDecl;
```

## Variable declaration

```ts
interface VariableDecl {
	name: Identifier;
	value: Expr;
	mutable: boolean;
	type?: Type;
}
```

---

## Function declaration

```ts
interface FunctionDecl {
	name: Identifier;
	args: Argument[];
	block: Block;
	returnType: Type;
}

interface Argument {
	name: Identifier;
	type: Type;
	optional: boolean;
}
```

---

## Structure declaration

```ts
interface StructureDecl {
	name: Identifier;
	params: Param[];
	methods: FunctionDecl[];
}

interface Param {
	name: Identifier;
	type: Type;
	public: boolean;
	optional: boolean;
	value?: Expr;
}
```

---

## Enumeration declaration

```ts
interface EnumDecl {
	name: Identifier;
	members: EnumMember[];
}

interface EnumMember {
	name: Identifier;
	number?: NumericLiteral;
}
```

---

# Statements

```ts
type Statement = 
	| IfStmt
	| SwitchStmt
	| DeferStmt
	| ReturnStmt
	| ThrowStmt
	| TryStmt
	| Loop
	| Declaration;
```

```ts
interface Block {
	statements: Statement[];
}
```

---

## If statement

```ts
interface IfStmt {
	condition: Expr;
	ifBlock: Block;
	elseIfs?: ElseIf[];
	else?: Block;
}

interface ElseIf {
	condition: Expr;
	block: Block;
}
```

---

## Switch statement

```ts
interface SwitchStmt {
	cases: Case[];
	wildcard?: Block;
}

interface Case {
	conditions: Expr[]; // allow (a, b, c)
	block: Block;
}
```

---

## Defer statement

```ts
interface DeferStmt {
	block: Block;
}
```

---

## Return statement

```ts
interface ReturnStmt {
	value?: Expr;
}
```

---

## Throw statement

```ts
interface ThrowStmt {
	value: Expr;
}
```

---

## Try / Catch statement

```ts
interface TryStmt {
	tryBlock: Block;
	catch: CatchClause;
}

interface CatchClause {
	param: Identifier;
	block: Block;
}
```

---

## Loops

```ts
type Loop = 
	| ForLoop
	| WhileLoop
	| UntilLoop
	| RepeatTimesLoop;
```

### For loop

```ts
interface ForLoop {
	variables: Identifier[];
	sequence: ForRange;
	block: Block;
}

type ForRange = 
	| FromToRange
	| Identifier
	
interface FromToRange {
	from: Expr;
	to: Expr;
}
```

---

# Expressions

```ts
type Expr =
	| BinaryExpr
	| UnaryExpr
	| Literal
	| MemberExpr
	| IndexExpr
	| FunctionCall
	| AssignmentExpr
	| FunctionExpr
	| NewExpr
	| SpawnExpr;
```

```ts
type Operator = string;
```

---

## Binary expression

```ts
interface BinaryExpr {
	left: Expr;
	operator: Operator;
	right: Expr;
}
```

---

## Unary expression (prefix & postfix)

```ts
interface UnaryExpr {
	operand: Expr;
	operator: Operator;
	postfix?: boolean;
}
```

Supports `-x`, `#x`, `x++`, `x--`.

---

## Assignment expression (compound)

```ts
interface AssignmentExpr {
	target: AssignTarget;
	operator?: Operator; // "=", "+=", "-=", etc.
	value: Expr;
}

type AssignTarget =
	| Identifier
	| MemberExpr
	| IndexExpr;
```

---

## Literals

```ts
type Literal = 
	| NumericLiteral
	| StringLiteral
	| InterpolatedString
	| BooleanLiteral
	| TagLiteral
	| ArrayLiteral
	| MapLiteral;
```

```ts
interface NumericLiteral { contents: number; }
interface StringLiteral { contents: string; }
interface BooleanLiteral { contents: boolean; }
interface TagLiteral { contents: string; }
```

### Interpolated string

```ts
interface InterpolatedString {
	parts: (string | Expr)[];
}
```

---

### Array literal

```ts
interface ArrayLiteral {
	elements: Expr[];
}
```

---

### Map literal

```ts
interface MapLiteral {
	entries: { key: Expr; value: Expr }[];
}
```

---

## Member expression (enum .Case)

```ts
interface MemberExpr {
	parent?: Identifier; // undefined for implicit enum like .Male
	memberName: Identifier;
}
```

---

## Index expression

```ts
interface IndexExpr {
	list: Expr;
	index: Expr;
}
```

---

## Function call

```ts
interface FunctionCall {
	function: Expr;
	arguments: CallArg[];
}

interface CallArg {
	name?: Identifier;
	value: Expr;
}
```

---

## Function expression (anonymous)

```ts
interface FunctionExpr {
	args: Argument[];
	block: Block;
	returnType: Type;
}
```

---

## New expression (constructors)

```ts
interface NewExpr {
	type: Type;
	arguments: Expr[];
}
```

---

## Spawn expression (tasks)

```ts
interface SpawnExpr {
	task: FunctionCall;
}
```

---

# Types

```ts
type Type = 
	| PrimitiveType
	| ArrayType
	| MapType
	| StructType
	| FunctionType;
```

---

## Primitive types

```ts
type PrimitiveType =
	| StringType
	| BooleanType
	| TagType
	| NumericType;

interface StringType {}
interface BooleanType {}
interface TagType {}

interface NumericType {
	kind: "int" | "uint" | "float";
	bitDepth?: 8 | 16 | 32 | 64;
}
```

---

## Array type

```ts
interface ArrayType {
	elementType: Type;
}
```

---

## Map type

```ts
interface MapType {
	keyType: Type;
	valueType: Type;
}
```

---

## Structure type

```ts
interface StructType {
	name: Identifier;
}
```

---

## Function type

```ts
interface FunctionType {
	args: Type[];
	return: Type;
}
```

---

# Internal

```ts
interface Identifier {
	name: string;
}
```
