import numpy as np
from tkinter import Tk, Canvas, messagebox, Label, Button
import random
from time import sleep
import subprocess

root = Tk()
width = 470
height = 700
offset1 = int(-width / 2 + root.winfo_screenwidth() / 2)
offset2 = int(-height / 2 + root.winfo_screenheight() / 2)
root.geometry(f'{width}x{height}+{offset1}+{offset2}')
root.resizable(width=False, height=False)
root.configure(bg='#ffffff')

piece_list = [[(2, 2)],
              [(2, 2), (3, 3)],
              [(2, 2), (3, 1)],
              [(2, 2), (2, 3)],
              [(2, 2), (3, 2)],
              [(2, 2), (1, 1), (3, 3)],
              [(2, 2), (3, 1), (1, 3)],
              [(2, 1), (2, 2), (2, 3)],
              [(2, 2), (1, 2), (3, 2)],
              [(2, 2), (1, 2), (2, 3)],
              [(2, 2), (1, 2), (2, 1)],
              [(2, 2), (3, 2), (2, 1)],
              [(2, 2), (3, 2), (2, 3)],
              [(2, 1), (2, 2), (2, 3), (2, 4)],
              [(2, 2), (1, 2), (3, 2), (4, 2)],
              [(2, 1), (2, 2), (2, 3), (3, 1)],
              [(2, 1), (2, 2), (2, 3), (1, 3)],
              [(2, 1), (2, 2), (2, 3), (1, 1)],
              [(2, 1), (2, 2), (2, 3), (3, 3)],
              [(2, 1), (2, 2), (2, 3), (1, 2)],
              [(2, 1), (2, 2), (2, 3), (3, 2)],
              [(2, 2), (1, 2), (3, 2), (3, 1)],
              [(2, 2), (1, 2), (3, 2), (1, 3)],
              [(2, 2), (1, 2), (3, 2), (1, 1)],
              [(2, 2), (1, 2), (3, 2), (3, 3)],
              [(2, 2), (1, 2), (3, 2), (2, 3)],
              [(2, 2), (1, 2), (3, 2), (2, 1)],
              [(2, 2), (3, 3), (2, 3), (3, 2)],
              [(2, 2), (2, 3), (3, 2), (3, 1)],
              [(2, 2), (3, 2), (2, 1), (3, 3)],
              [(2, 2), (2, 3), (3, 2), (1, 3)],
              [(2, 2), (3, 2), (2, 1), (1, 1)],
              [(0, 2), (1, 2), (2, 2), (3, 2), (4, 2)],
              [(2, 0), (2, 1), (2, 2), (2, 3), (2, 4)],
              [(2, 1), (1, 2), (2, 2), (3, 2), (2, 3)],
              [(3, 1), (2, 1), (2, 2), (2, 3), (3, 3)],
              [(1, 1), (2, 1), (2, 2), (2, 3), (1, 3)],
              [(1, 1), (1, 2), (2, 2), (3, 2), (3, 1)],
              [(1, 3), (1, 2), (2, 2), (3, 2), (3, 3)],
              [(3, 3), (1, 2), (2, 2), (3, 2), (3, 1)],
              [(1, 3), (1, 2), (2, 2), (3, 2), (1, 1)],
              [(1, 1), (2, 1), (2, 2), (2, 3), (3, 1)],
              [(3, 3), (2, 1), (2, 2), (2, 3), (1, 3)],
              [(1, 3), (2, 3), (3, 3), (3, 2), (3, 1)],
              [(3, 1), (2, 1), (1, 1), (1, 2), (1, 3)],
              [(3, 3), (2, 3), (1, 3), (1, 2), (1, 1)],
              [(1, 1), (2, 1), (3, 1), (3, 2), (3, 3)],
              ]


class Board:
    def __init__(self):
        self.state = np.zeros((17, 17))

    def small_board(self):
        result = np.zeros((9, 9))
        for i in range(4, 13):
            for j in range(4, 13):
                result[i - 4][j - 4] = self.state[i][j]
        return result

    def print_board(self):
        print(self.small_board())

    def generate_moves(self, piece):
        result = []
        for i in range(13):
            for j in range(13):
                add_move = True
                for square in piece:
                    if 13 > i + square[0] > 3 and 13 > j + square[1] > 3:
                        if self.state[i + square[0]][j + square[1]] == 1:
                            add_move = False
                            break
                    else:
                        add_move = False
                        break
                if add_move:
                    result.append((i, j))

        return result

    def place_piece(self, piece, location):
        for square in piece:
            self.state[location[0] + square[0]][location[1] + square[1]] = 1

    def remove_piece(self, piece, location):
        for square in piece:
            self.state[location[0] + square[0]][location[1] + square[1]] = 0

    def clear_board(self):
        to_clear = []
        bonus = 0
        for i in range(9):
            clear = True
            for j in range(9):
                if self.state[4 + i][4 + j] == 0:
                    clear = False
                    break
            if clear:
                bonus += 16
                for j in range(9):
                    to_clear.append((4 + i, 4 + j))

        for i in range(9):
            clear = True
            for j in range(9):
                if self.state[4 + j][4 + i] == 0:
                    clear = False
                    break
            if clear:
                bonus += 16
                for j in range(9):
                    to_clear.append((4 + j, 4 + i))

        for a in range(3):
            for b in range(3):
                clear = True
                for i in range(3):
                    for j in range(3):
                        if self.state[3 * a + 4 + j][3 * b + 4 + i] == 0:
                            clear = False
                if clear:
                    bonus += 16
                    for i in range(3):
                        for j in range(3):
                            to_clear.append((3 * a + 4 + j, 3 * b + 4 + i))
        for square in to_clear:
            self.state[square[0]][square[1]] = 0

        return bonus

    def empty_board(self):
        for i in range(17):
            for j in range(17):
                self.state[i][j] = 0


class BruteForce:
    def __init__(self, path_engine: str = "Engine Path"):
        print("Log: engine started.")
        # Open the engine executable
        self.engine = subprocess.Popen(path_engine, universal_newlines=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)

    def _put(self, command):
        # Write commands to the engine
        if not self.engine.stdin:
            raise BrokenPipeError()
        self.engine.stdin.write(f"{command}\n")
        self.engine.stdin.flush()

    def _read_line(self):
        # Read engine output
        if not self.engine.stdout:
            raise BrokenPipeError()
        return self.engine.stdout.readline().strip()

    def play_move(self, state, pieces):
        self._put(piece_list.index(pieces[0]) + 1)
        self._put(piece_list.index(pieces[1]) + 1)
        self._put(piece_list.index(pieces[2]) + 1)
        for i in range(4, 13):
            for j in range(4, 13):
                self._put(int(state.state[i][j]))

        last_line = ""
        while True:
            line = self._read_line()
            if line == "Engine done.":
                break
            if line == "Cannot avoid loss.":
                print(line)
                return "Dead position"
            last_line = line

        last_line = last_line.split(" ")
        last_line = [int(x) for x in last_line]
        return last_line

    def engine_kill(self):
        print("Log: engine terminated.")
        self.engine.terminate()


class Interface:
    def __init__(self, setup_board):
        self.grey = Canvas(root, width='449', height='449', highlightthickness=2, highlightbackground='#001347')
        self.grey.place(x=10, y=75)
        self.last = (-4, -4)
        self.selection = Canvas(root, width='449', height='150', highlightthickness=2, highlightbackground='#001347',
                                bg='#ffffff')
        self.selection.place(x=10, y=540)
        self.score = 0
        self.score_text = Label(root, text='Score: ' + str(self.score), bg='#ffffff', font=("calibri bold", 30))
        self.score_text.place(x=10, y=8)
        self.state = setup_board
        # self.small_board = self.state.small_board()
        self.selectable_pieces = [(), (), ()]
        self.masked_pieces = []
        self.selected_piece = -1
        # show initial board
        self.display_board()
        self.selection.bind('<Button-1>', self.pick_piece)
        self.grey.bind('<Button-1>', self.drop_piece)
        self.grey.bind('<Motion>', self.movement)

        # generate engine terminal
        root.protocol("WM_DELETE_WINDOW", self.close)

        self.engine_player = BruteForce('blockudoku')
        self.start_engine = Button(root, text="Start Engine", bg='#ffffff', font=("calibri bold", 15), command=lambda: self.engine_move())
        self.start_engine.place(x=345, y=18)

    def close(self):
        if messagebox.askokcancel("Quit", "Do you want to quit?"):
            self.engine_player.engine_kill()
            root.destroy()

    def display_board(self):
        self.score_text.config(text='Score: ' + str(self.score))
        self.grey.delete('all')
        self.selection.delete('all')
        local_small_board = self.state.small_board()

        for i in range(9):
            for j in range(9):
                if ((j // 3) % 2 + (i // 3) % 2) % 2 == 0:
                    self.grey.create_rectangle((50 * j, 50 * i, 50 + 50 * j, 50 + 50 * i),
                                               fill='#ffffff', outline='#d2d8df')
                else:
                    self.grey.create_rectangle((50 * j, 50 * i, 50 + 50 * j, 50 + 50 * i),
                                               fill='#e4e9ef', outline='#d2d8df')
        if self.last != (-4, -4):
            for square in self.selectable_pieces[self.selected_piece]:
                j = square[1] + self.last[0]
                i = square[0] + self.last[1]
                if ((j // 3) % 2 + (i // 3) % 2) % 2 == 0:
                    self.grey.create_rectangle((50 * j, 50 * i, 50 + 50 * j, 50 + 50 * i),
                                               fill='#cdcccf', outline='#d2d8df')
                else:
                    self.grey.create_rectangle((50 * j, 50 * i, 50 + 50 * j, 50 + 50 * i),
                                               fill='#b6bbc1', outline='#d2d8df')

        for i in range(9):
            for j in range(9):
                if local_small_board[j][i] == 1:
                    self.grey.create_rectangle((50 * i, 50 * j, 50 + 50 * i, 50 + 50 * j),
                                               fill='#2a57b4', outline='#001347')
        for i in range(3):
            if i in self.masked_pieces:
                continue
            color_piece = '#2a57b4'
            color_edge = '#001347'
            if len(self.state.generate_moves(self.selectable_pieces[i])) == 0:
                color_piece = '#d8e3ff'
                color_edge = '#cad2df'
            for square in self.selectable_pieces[i]:
                self.selection.create_rectangle((i * 150 + 13 + 25 * square[1], 13 + 25 * square[0],
                                                 i * 150 + 38 + 25 * square[1], 38 + 25 * square[0]),
                                                fill=color_piece, outline=color_edge)
        root.update()

    def pick_piece(self, event):
        if event.x // 150 not in self.masked_pieces:
            self.selected_piece = event.x // 150

    def drop_piece(self, event):
        if not 3 > self.selected_piece >= 0:
            return
        move = (event.y // 50 + 2, event.x // 50 + 2)
        moves = self.state.generate_moves(self.selectable_pieces[self.selected_piece])
        if move in moves:
            print(move)
            self.state.place_piece(self.selectable_pieces[self.selected_piece], move)
            self.masked_pieces.append(self.selected_piece)
            self.score += self.state.clear_board() + len(self.selectable_pieces[self.selected_piece])
            self.last = (-4, -4)
            if len(self.masked_pieces) == 3:
                self.reset()
            self.display_board()
            self.check_terminal()
            self.selected_piece = -1
        else:
            self.selected_piece = -1

    def reset(self):
        self.masked_pieces = []
        self.selected_piece = -1
        self.selectable_pieces = [(), (), ()]
        self.display_board()
        self.selectable_pieces = [random.choice(piece_list), random.choice(piece_list), random.choice(piece_list)]
        sleep(0.25)
        self.display_board()

    def check_terminal(self):
        terminal = True
        for i in range(3):
            if i not in self.masked_pieces:
                if len(self.state.generate_moves(self.selectable_pieces[i])) != 0:
                    terminal = False
        if terminal:
            messagebox.showinfo(title='Result', message='Game has ended.\nScore: ' + str(self.score))
            self.score = 0
            self.state.empty_board()
            self.reset()

    def movement(self, event):
        if self.selected_piece != -1 and self.selected_piece not in self.selectable_pieces:
            if self.last != (event.x // 50 - 2, event.y // 50 - 2):
                move = (event.y // 50 + 2, event.x // 50 + 2)
                moves = self.state.generate_moves(self.selectable_pieces[self.selected_piece])
                if move in moves:
                    self.last = (event.x // 50 - 2, event.y // 50 - 2)
                else:
                    self.last = (-4, -4)
                self.display_board()

    def engine_move(self):
        if len(self.masked_pieces) == 0:
            for i in range(10000):
                sequence = self.engine_player.play_move(self.state, self.selectable_pieces)
                if sequence == "Dead position":
                    return

                for i in range(0, 9, 3):
                    # print(self.selectable_pieces[sequence[i]], [sequence[i+1], sequence[i+2]])
                    self.state.place_piece(self.selectable_pieces[sequence[i]], [sequence[i+1], sequence[i+2]])
                    self.masked_pieces.append(sequence[i])
                    self.display_board()
                    sleep(0.2)
                    self.score += self.state.clear_board() + len(self.selectable_pieces[sequence[i]])

                    self.display_board()
                    sleep(0.2)
                self.reset()
        else:
            print("Finish move first")


if __name__ == '__main__':
    board = Board()
    board.clear_board()

    application = Interface(board)
    application.reset()

root.mainloop()
