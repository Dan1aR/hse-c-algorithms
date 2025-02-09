from dataclasses import dataclass
from typing import List, Optional, Union
from enum import Enum

from tokenizer import Token


# AST Node Classes
class NodeType(Enum):
    PROGRAM = "PROGRAM"
    FUNCTION = "FUNCTION"
    BLOCK = "BLOCK"
    STATEMENT = "STATEMENT"  # Added missing STATEMENT type
    ASSIGNMENT = "ASSIGNMENT"
    BINARY_OP = "BINARY_OP"
    FUNCTION_CALL = "FUNCTION_CALL"
    NUMBER = "NUMBER"
    IDENTIFIER = "IDENTIFIER"
    WILDCARD = "WILDCARD"


@dataclass
class ASTNode:
    type: NodeType
    line: int
    column: int


@dataclass
class Program(ASTNode):
    functions: List["Function"]
    statements: List[Union["Statement", "Block"]]


@dataclass
class Function(ASTNode):
    name: str
    body: List[Union["Statement", "Block"]]


@dataclass
class Block(ASTNode):
    block_type: str  # '@' or '?' or '?!'
    condition: "Expression"
    statements: List["Statement"]


@dataclass
class Statement(ASTNode):
    expression: Union["Expression", "Wildcard"]
    target: Optional[str] = None  # For assignments
    is_return: bool = False  # True if this is a return/stop statement (*)


@dataclass
class Expression(ASTNode):
    pass


@dataclass
class BinaryOp(Expression):
    operator: str
    left: Union["Expression", "Value"]
    right: Union["Expression", "Value"]


@dataclass
class Value(Expression):
    pass


@dataclass
class Number(Value):
    value: int


@dataclass
class Identifier(Value):
    name: str


@dataclass
class FunctionCall(Value):
    name: str
    argument: Optional["Expression"]


@dataclass
class Wildcard(ASTNode):
    pass


class ParserError(Exception):
    pass


class Parser:
    def __init__(self, tokens: List["Token"]):
        self.tokens = tokens
        self.current = 0

    def peek(self) -> Optional["Token"]:
        if self.current < len(self.tokens):
            token = self.tokens[self.current]
            # # print(f"DEBUG Peek: {token}")
            return token
        # print("DEBUG Peek: End of tokens")
        return None

    def advance(self) -> Optional["Token"]:
        if self.current < len(self.tokens):
            token = self.tokens[self.current]
            self.current += 1
            return token
        return None

    def match(self, token_type: str) -> bool:
        token = self.peek()
        return token is not None and token.type == token_type

    def expect(self, token_type: str) -> "Token":
        token = self.peek()
        if token is None or token.type != token_type:
            raise ParserError(
                f"Expected {token_type}, got {token.type if token else 'EOF'}"
            )
        return self.advance()

    def parse_program(self) -> Program:
        functions = []
        statements = []

        while self.peek():
            if self.match("FUNCTION_MARKER"):
                functions.append(self.parse_function())
            else:
                stmt = self.parse_statement_any()
                if stmt:
                    statements.append(stmt)

        return Program(
            type=NodeType.PROGRAM,
            functions=functions,
            statements=statements,
            line=1,
            column=1,
        )

    def parse_function(self) -> Function:
        # print(f"DEBUG Entering parse_function at position {self.current}")

        # Function definition
        start_token = self.expect("FUNCTION_MARKER")
        name_token = self.expect("NAME")
        self.expect("NEWLINE")  # Function definition must end with newline

        # Parse function body
        body = []
        while self.peek():
            current_token = self.peek()
            # Check if we're still in function body (indent of 4)
            if current_token.column != 5:  # 4 spaces + 1 (1-based column indexing)
                break

            # Parse statement at correct indentation
            stmt = self.parse_statement_any()
            if stmt:
                body.append(stmt)

        if not body:
            raise ParserError(f"Empty function body in function {name_token.value}")

        return Function(
            type=NodeType.FUNCTION,
            name=name_token.value,
            body=body,
            line=start_token.line,
            column=start_token.column,
        )

    def parse_statement_any(self) -> Optional[Union[Statement, Block]]:
        # print(f"DEBUG Entering parse_statement_any, current token: {self.peek().type}")

        if self.match("LOOP_MARKER") or self.match("IF_MARKER"):
            block = self.parse_block()
            # BLOCK rule already ensures it ends with newline
            return block
        else:
            stmt = self.parse_statement_one()
            if stmt:
                # STATEMENT_ONE must be followed by newline
                self.expect("NEWLINE")
            return stmt

    def parse_block(self) -> Block:
        # print("DEBUG Entering parse_block")
        start_token = self.advance()  # @ or ? or ?!
        block_type = start_token.value

        condition = self.parse_expression()
        self.expect("COLON")

        # print("DEBUG Parsing block statements")
        statements = []
        while self.peek() and not self.match("NEWLINE"):
            # print("-" * 30)
            # print(f"DEBUGP arsing in block, current token: {self.peek().type}")
            stmt = self.parse_statement_one()
            # print("DEBUG BLOCK parsed statement:")
            # print("-" * 30)
            if stmt:
                statements.append(stmt)
            if self.match("SEMICOLON"):
                self.advance()
                # After semicolon, there might be more statements
                if self.match("NEWLINE"):
                    break
            else:
                # If no semicolon, this must be the last statement
                # raise ParserError(f"No semicolon after statement in block")
                break

        # print(f"DEBUG Block parsed with {len(statements)} statements")
        self.expect("NEWLINE")
        return Block(
            type=NodeType.BLOCK,
            block_type=block_type,
            condition=condition,
            statements=statements,
            line=start_token.line,
            column=start_token.column,
        )

    def parse_statement_one(self) -> Optional[Statement]:
        # print(f"DEBUG Entering parse_statement_one at position {self.current}")

        # Check for standalone * operator (return/stop statement)
        if self.match("OPERATOR") and self.peek().value == "*":
            # print("DEBUG Found standalone * operator")
            star_token = self.advance()
            # Make sure it's not part of a multiplication expression
            next_token = self.peek()
            if next_token and next_token.type in ["SEMICOLON", "NEWLINE"]:
                return Statement(
                    type=NodeType.STATEMENT,
                    expression=Wildcard(
                        type=NodeType.WILDCARD,
                        line=star_token.line,
                        column=star_token.column,
                    ),
                    line=star_token.line,
                    column=star_token.column,
                    is_return=True,  # Mark as return/stop statement
                )
            else:
                # If * is followed by other tokens, backtrack to parse as multiplication
                self.current -= 1

        target = None
        if self.match("NAME"):
            # print("DEBUG Found NAME token")
            name_token = self.advance()
            if self.match("EQUALS"):
                # print("DEBUG Found EQUALS after NAME - this is an assignment")
                self.advance()
                target = name_token.value
            else:
                # print("DEBUG No EQUALS after NAME - backtracking")
                self.current -= 1  # backtrack if no assignment

        expression = self.parse_expression()
        return Statement(
            type=NodeType.STATEMENT,
            target=target,
            expression=expression,
            line=expression.line,
            column=expression.column,
        )

    def parse_expression(self) -> Expression:
        # print(f"DEBUG Entering parse_expression at position {self.current}")
        left = self.parse_muldiv()

        while self.peek() and self.peek().value in ["+", "-"]:
            # print(f"DEBUG Found operator {self.peek().value}")
            operator = self.advance().value
            right = self.parse_muldiv()
            left = BinaryOp(
                type=NodeType.BINARY_OP,
                operator=operator,
                left=left,
                right=right,
                line=left.line,
                column=left.column,
            )

        return left

    def parse_muldiv(self) -> Expression:
        left = self.parse_value()

        while self.peek() and self.peek().value in ["*", "/"]:
            operator = self.advance().value
            right = self.parse_value()
            left = BinaryOp(
                type=NodeType.BINARY_OP,
                operator=operator,
                left=left,
                right=right,
                line=left.line,
                column=left.column,
            )

        return left

    def parse_value(self) -> Value:
        token = self.peek()

        if self.match("LPAREN"):
            self.advance()
            expr = self.parse_expression()
            self.expect("RPAREN")
            return expr

        elif self.match("NUMBER"):
            token = self.advance()
            return Number(
                type=NodeType.NUMBER,
                value=int(token.value),
                line=token.line,
                column=token.column,
            )

        elif self.match("NAME"):
            name_token = self.advance()
            if self.match("FUNCTION_MARKER"):
                self.advance()
                self.expect("LPAREN")
                arg = None
                if not self.match("RPAREN"):
                    arg = self.parse_expression()
                self.expect("RPAREN")
                return FunctionCall(
                    type=NodeType.FUNCTION_CALL,
                    name=name_token.value,
                    argument=arg,
                    line=name_token.line,
                    column=name_token.column,
                )
            return Identifier(
                type=NodeType.IDENTIFIER,
                name=name_token.value,
                line=name_token.line,
                column=name_token.column,
            )

        raise ParserError(f"Unexpected token: {token}")


def parse(tokens: List["Token"]) -> Program:
    parser = Parser(tokens)
    return parser.parse_program()
