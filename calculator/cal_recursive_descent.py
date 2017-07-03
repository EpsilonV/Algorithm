import operator
INTEGER, PLUS, MINUS,MUL, DIV, LPARE, RPARE, EOF = 'INTEGER', 'PLUS', 'MINUS', 'MUL', 'DIV', 'LPARE', 'RPARE' ,"EOF"

class Token():
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(type=self.type, value=repr(self.value))

    def __repr__(self):
        return self.__str__()

class Lexer():
    def __init__(self, text):
        self._text = text
        self.pos = 0
        self.current_char = self._text[self.pos]

    def error(self):
        raise Exception('Invalid character')

    def advanced(self):
        self.pos += 1
        if self.pos > len(self._text) - 1:
            self.current_char = None
        else:
            self.current_char = self._text[self.pos]
    
    def skip_whitespace(self):
        while self.current_char is not None and self.current_char == ' ':
            self.advanced()

    def integer(self):
        result = ''
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.advanced()
        return int(result)

    def get_next_token(self):
        while self.current_char is not None:
            if self.current_char == ' ':
                self.skip_whitespace()
                continue

            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())

            if self.current_char == '*':
                self.advanced()
                return Token(MUL, '*')

            if self.current_char == '/':
                self.advanced()
                return Token(DIV, '/')

            if self.current_char == '+':
                self.advanced()
                return Token(PLUS, '+')

            if self.current_char == '-':
                self.advanced()
                return Token(MINUS, '-')

            if self.current_char == '(':
                self.advanced()
                return Token(LPARE, '(')

            if self.current_char == ')':
                self.advanced()
                return Token(RPARE, ')')

            self.error()
        return Token(EOF, None)

class Interpreter():
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()
        self.opMapping = {
            PLUS: operator.add,
            MINUS: operator.sub,
            MUL: operator.mul,
            DIV: operator.truediv
        }

    def error(self):
        raise Exception('Invalid syntax')

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    def number(self):
        value = self.current_token.value
        self.eat(INTEGER)
        return value
    
    def factor(self):
        """factor : INTEGER"""
        if self.current_token.type == LPARE:
            self.eat(LPARE)
            result = self.expr()
            self.eat(RPARE)
        else:
            result = self.current_token.value
            self.eat(INTEGER)
        return result

    def term(self):
        """term : factor ((MUL | DIV) factor)*"""
        result = self.factor()

        while self.current_token.type in (MUL, DIV):
            token = self.current_token
            if token.type == MUL:
                self.eat(MUL)
                result = result * self.factor()
            elif token.type == DIV:
                self.eat(DIV)
                result = result / self.factor()

        return result

    def expr(self):
        """Arithmetic expression parser / interpreter.
        grammar:
            expr   : term ((PLUS | MINUS) term)*
            term   : factor ((MUL | DIV) factor)*
            factor : INTEGER | LPARE trem RPARE
        """
        result = self.term()

        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            if token.type == PLUS:
                self.eat(PLUS)
                result = result + self.term()
            elif token.type == MINUS:
                self.eat(MINUS)
                result = result - self.term()

        return result



def main():
    while True:
        try:
            text = input('calc> ')
        except EOFError:
            break
        if not text:
            continue
        lexer = Lexer(text)
        interpreter = Interpreter(lexer)
        result = interpreter.expr()
        print(result)

if __name__ == '__main__':
    main()
