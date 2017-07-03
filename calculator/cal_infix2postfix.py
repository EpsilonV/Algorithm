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

class Calculator():
    def __init__(self, lexer):
        self.lexer = lexer
        self.tokens = self.getAllToken()

    def getAllToken(self):
        tokens = []
        token = self.lexer.get_next_token()
        while token.type != EOF:
            tokens.append(token)
            token = self.lexer.get_next_token()
        
        return tokens

    def infixToPostfix(self):
        prec = {
            MUL: 3,
            DIV: 3,
            PLUS: 2,
            MINUS: 2,
            LPARE: 1
        }

        opStack = []
        postfixList = []

        for token in self.tokens:
            if token.type == INTEGER:
                postfixList.append(token)
            elif token.type == LPARE:
                opStack.append(token)
            elif token.type == RPARE:
                top = opStack.pop()
                while top.type != LPARE:
                    postfixList.append(top)
                    top = opStack.pop()
            else:
                while len(opStack) > 0 and prec[opStack[len(opStack)-1].type] >= prec[token.type]:
                    postfixList.append(opStack.pop())
                opStack.append(token)
        
        if len(opStack) > 0:
            postfixList[len(postfixList):] = opStack[::-1]
        
        return postfixList

    def expr(self):
        operandStack = []
        postfixTokens = self.infixToPostfix()

        opMapping = {
            MUL: operator.mul,
            DIV: operator.truediv,
            PLUS: operator.add,
            MINUS: operator.sub
        }

        for token in postfixTokens:
            if token.type == INTEGER:
                operandStack.append(token.value)
            else:
                right = operandStack.pop()
                left = operandStack.pop()
                result = opMapping[token.type](left, right)
                operandStack.append(result)
        
        return operandStack.pop()


def main():
    while True:
        try:
            text = input('calc> ')
        except EOFError:
            break
        if not text:
            continue
        lexer = Lexer(text)
        cal = Calculator(lexer)
        print(cal.expr())

if __name__ == '__main__':
    main()