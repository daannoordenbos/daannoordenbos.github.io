---
title: "Projects"
permalink: "/projects/"
layout: page
---

This page will contain a list of projects that I am working on or have worked on.

# PGN to markdown parser
A very useful project for me personally, it parses a .pgn file so that it is in the right markdown format to be displayed on this site. The post about this project can be found [here](https://daannoordenbos.github.io/PGN-parser-for-markdown/).

# Game playing agents
## Draughts
The first game playing agent I developed plays draughts, specifically international draughts played on the 10x10 board. The initial focus was on improving the speed and search depth of the move generator and alpha-beta search. With a quite common evaluation function it kept GMI Martin Dolfing to a draw ([game 1](https://lidraughts.org/tJ21UR4b/black), [game 2](https://lidraughts.org/iaELUv9a/white)). Further development focused on using Monte Carlo tree search (MCTS) and different draughts variants. 

## Blockudoku
Blockudoku is a puzzle game and it is described as “An original combination of Sudoku and block puzzle games”. The goal of this project was to create an agent that was able to outperform humans in the game of Blockudoku. This project marked my first time of dealing with stochasticity in a game and my first time (intentionally) using OOP in Python. While I had big plans for the project, it turned out that very little is required to essentially 'beat' the puzzle. A full report of the report can be read [here](https://daannoordenbos.github.io/Blockudoku-Agent/).

# Stratego agent
Stratego is a hidden information based boardgame. Stratego is a very hard game for a computer to play well, because of the particularities of the game. Most importantly it is a plan based game where the plans can take many moves to execute and these moves can often be executed in no particular order. The result is that typical search methods are unsuitable for the game of Stratego. The goal of this project was to create an agent that is able to beat humans. However I was not able to create an agent capable of beating me (a beginner).
## Amazons
The Game of the Amazons, or Amazons for short, is a two-player abstract strategy game. The game is played by moving pieces and blocking the opponents from squares, and the last player able to move is the winner. It is a member of the territorial game family, a distant relative of Go and chess. I focused on the variant that is played on an 8x8 board. Due to the very high branching factor usual search methods cannot go deep. So the current best program used MCTS and a special endgame routine.
