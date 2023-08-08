from chess import pgn, svg


class MarkdownChess:
    def __init__(self, fileName):
        self.game = pgn.read_game(open(fileName))
        self.extract = ''
        self.lastActionNewline = False
        self.mainline(self.game.variations[0], True, 0)

    def mainline(self, mainline, addMove, depth):
        if addMove:
            self.addMove(mainline, False, depth)
        if mainline.is_end():
            return
        if len(mainline.variations) > 1:
            self.addMove(mainline.variations[0], True, depth)
            for sideline in mainline.variations[1:]:
                self.mainline(sideline, True, depth + 1)

        self.mainline(mainline.variations[0], len(mainline.variations) == 1, depth)

    def addMove(self, node, newLine, depth):
        formatting = f'{"*" if depth == 0 else ""}' + f'{"*" if depth % 2  == 0 else ""}'
        self.extract += formatting
        # Move indicator
        if node.ply() % 2 == 1 or self.lastActionNewline:
            self.extract += str((node.ply() + 1) // 2) + f'{"." if node.ply() % 2 == 1 else "..."}'
        # Short Algebraic Notation
        unicode = {'R': '&#9814;', 'N': '&#9816;', 'B': '&#9815;', 'Q': '&#9813;', 'K': '&#9812;'}
        self.extract += f'{unicode[str(node.san())[0]] + str(node.san())[1:] if str(node.san())[0] in unicode else str(node.san())}'
        # Numeric Annotation Glyphs (NAGs)
        nags = ['', '!', '?', '!!', '??', '!?', '?!', '', '', '', '=', '', '', '', '+/=', '-/=', '+/-', '-/+', '+-', '-+']
        for nag in node.nags:
            if nag < 20:
                self.extract += nags[nag]
        self.extract += formatting + ' '

        if node.comment:
            self.extract += node.comment.replace("\n", " ") + ' '
            # SVG boards can be called by [D].
            if node.comment.find('[D]') != -1:
                board_code = svg.board(node.board(), lastmove=node.move)
                board_code = board_code.replace('viewBox="0 0 390 390"', 'viewBox="-97 0 584 390"')

                self.extract = self.extract.replace("[D]", "\n" + board_code)

        self.lastActionNewline = False
        if newLine or node.is_end():
            self.extract += "\n\n"
            self.lastActionNewline = True

    def saveAsTemporary(self, fileName='log.txt'):
        print(self.extract, file=open(fileName, 'w'))

    def __repr__(self):
        return self.extract


if __name__ == "__main__":
    post = MarkdownChess("Far pawn endgame.pgn")

    post.saveAsTemporary()
    print(post)
