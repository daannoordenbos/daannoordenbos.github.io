from chess import pgn, svg

extract = ''


def parse_game(file_name):
    global extract
    pgn_file = open(file_name)

    main_game = pgn.read_game(pgn_file)

    extract = ''

    def add_node(node, depth, jump):
        global extract

        unicode = {
            'R': '&#9814;',
            'N': '&#9816;',
            'B': '&#9815;',
            'Q': '&#9813;',
            'K': '&#9812;'
        }

        if node.starts_variation():
            extract += '\n\n'
        # take care of bold/italic levels
        if node.is_mainline():
            extract += '**'
        if depth % 2 == 1:
            extract += '*'
        # take care of move number
        if node.ply() % 2 == 1 or node.starts_variation() or jump != 0:
            if node.ply() % 2 == 1:
                extract += str((node.ply() + 1) // 2) + '.'
            else:
                extract += str((node.ply() + 1) // 2) + '...'
        # unicode pieces
        try:
            extract += unicode[str(node.san())[0]]
            extract += str(node.san())[1:]
        except KeyError:
            extract += str(node.san())
        # insert nags
        nags = ['', '!', '?', '!!', '??', '!?', '?!', '', '', '', '=', '', '', '', '+/=', '-/=', '+/-', '-/+', '+-',
                '-+']
        for nag in node.nags:
            if nag < 20:
                extract += nags[nag]
        # take care of bold/italic levels
        if depth % 2 == 1:
            extract += '*'
        if node.is_mainline():
            extract += '** '
        else:
            extract += ' '
        # add stripped comment
        if node.comment:
            extract += node.comment.replace("\n", " ") + ' '
            # add rendered boards into the file as code
            if node.comment.find('[D]') != -1:
                board_code = svg.board(node.board(), lastmove=node.move)
                board_code = board_code.replace('viewBox="0 0 390 390"', 'viewBox="-97 0 584 390"')

                extract = extract.replace("[D]", "\n" + board_code)

    def traverse(node, show_move, depth=0, var_num=0, par_var=0):
        global extract
        # do whatever stuff you want for the node here
        if node.ply() > 0 and show_move:
            add_node(node, depth, var_num - par_var)
        # terminating condition
        if node.is_end():
            extract += '\n'
            return
        # recursion with proper order and depth
        i = 1
        branches = len(node.variations)
        if branches == 1:
            for child_node in reversed(node.variations):
                traverse(child_node, True, depth + int(i < branches), i, var_num)
                i += 1
        else:
            add_node(node.variations[0], depth, var_num - par_var)

            for child_node in reversed(node.variations):
                traverse(child_node, i < branches, depth + int(i < branches), i, var_num)
                i += 1

    extract += '# ' + main_game.headers["White"] + ' (' + main_game.headers["WhiteElo"] + ') - '
    extract += main_game.headers["Black"] + ' (' + main_game.headers["BlackElo"] + ')\n## '
    extract += main_game.headers["Event"] + ' (' + main_game.headers["Round"] + '), ' + main_game.headers["Date"] + "\n"
    traverse(main_game, True)
    extract += ' ' + main_game.headers['Result']

    print(extract, file=open('log.txt', 'w'))

    return extract


print(parse_game("Far pawn endgame.pgn"))
