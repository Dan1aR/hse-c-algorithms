import re
from dataclasses import dataclass
from typing import List, Optional


@dataclass
class Token:
    type: str
    value: str
    line: int
    column: int

    def __repr__(self):
        _val = self.value if self.value != "\n" else "\\n"
        return f"Token:{self.type:<20}{_val:<10}pos=[{self.line}, {self.column}]"


class Tokenizer:
    def __init__(self):
        # Token patterns
        self.token_patterns = [
            ("WHITESPACE", r"[ \t]+"),
            ("NEWLINE", r"\n"),
            ("FUNCTION_MARKER", r"!"),
            ("LOOP_MARKER", r"@"),
            ("IF_MARKER", r"\?!?"),  # Matches both ? and ?!
            ("OPERATOR", r"[+\-*/]"),
            ("EQUALS", r"="),
            ("LPAREN", r"\("),
            ("RPAREN", r"\)"),
            ("COLON", r":"),
            ("SEMICOLON", r";"),
            ("WILDCARD", r"\*"),
            ("NUMBER", r"\d+"),
            ("NAME", r"[a-zA-Z_][a-zA-Z0-9_]*"),
        ]

        # Combine patterns into a single regex
        self.pattern = "|".join(
            f"(?P<{name}>{pattern})" for name, pattern in self.token_patterns
        )
        self.regex = re.compile(self.pattern)

    def remove_comments(self, text: str) -> str:
        """Remove comments and empty lines from the text."""
        lines = []
        for line in text.splitlines():
            # Remove comment and trailing whitespace
            cleaned_line = line.split("#")[0].rstrip()
            # Only keep non-empty lines
            if cleaned_line:
                lines.append(cleaned_line)
        # Join with newlines and add final newline
        return "\n".join(lines) + "\n"

    def tokenize(self, text: str) -> List[Token]:
        """Convert input text into a list of tokens."""
        # Remove comments first
        text = self.remove_comments(text)
        # print("DEBUG Text after comment removal (showing newlines as \\n):")
        # print(repr(text))
        tokens = []

        line_num = 1
        column_num = 1

        pos = 0
        while pos < len(text):
            match = self.regex.match(text, pos)

            if match is None:
                raise ValueError(
                    f"Invalid character at line {line_num}, column {column_num}: {text[pos]}"
                )

            token_type = match.lastgroup
            token_value = match.group()

            # print(f"DEBUG Found token: {token_type} '{token_value}' at pos {pos}")

            # Skip whitespace but keep track of position
            if token_type == "WHITESPACE":
                column_num += len(token_value)
            elif token_type == "NEWLINE":
                # Always create token for newline
                tokens.append(Token(token_type, token_value, line_num, column_num))
                line_num += 1
                column_num = 1
                # print(f"DEBUG Added NEWLINE token at line {line_num-1}")
            else:
                # Create token for non-whitespace matches
                tokens.append(Token(token_type, token_value, line_num, column_num))
                column_num += len(token_value)

            pos = match.end()

        return tokens
