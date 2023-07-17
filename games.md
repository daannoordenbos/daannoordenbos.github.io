---
title: "Games"
permalink: "/games/"
layout: page
---

As a pastime, I develop game-playing agents capable of playing and analyzing various games. This page provides a brief overview of some of my projects, along with links to more detailed posts about each of them.

## Draughts
The first game playing agent I developed plays draughts, specifically international draughts played on the 10x10 board. The initial focus was on improving the speed and search depth of the [move generator](https://daannoordenbos.github.io/draughts-move-generator/) and alpha-beta search. With a quite common evaluation function it kept GMI Martin Dolfing to two draws ([game 1](https://lidraughts.org/tJ21UR4b/black), [game 2](https://lidraughts.org/iaELUv9a/white)). Further development focused on using Monte Carlo tree search (MCTS) and different draughts variants. 

## Blockudoku
Blockudoku, an engaging puzzle game, is described as “An original combination of Sudoku and block puzzle games”. The goal of this project was to create an agent that was able to outperform humans in the game of Blockudoku. This project marked my first time of dealing with stochasticity in a game and my first time (intentionally) using OOP in Python. While I had big plans for the project, it turned out that very little is required to essentially 'beat' the puzzle. A full report of the report can be read [here](https://daannoordenbos.github.io/Blockudoku-Agent/).

## Stratego agent
Stratego is a board game involving hidden information. It poses a significant challenge for computer players due to its unique characteristics. The game relies heavily on strategic planning, often spanning multiple moves, which can be executed in any order. Consequently, typical search methods are unsuitable for Stratego. My goal for this project was to create an agent capable of defeating human opponents. However, due to the game's complexity, achieving this goal remained elusive. As a side project, I developed a [program](https://daannoordenbos.github.io/Bradley-Terry-Intervals/) to determine the maximum likelihood estimates of the agents' Elo ratings to assess their relative performance.

## Amazons
The Game of the Amazons, or simply Amazons, is an abstract strategy game played by two players. The objective is to move pieces and block opponents from squares, with the last player able to move declared the winner. This game variant focuses on an 8x8 board. The high branching factor in Amazons renders conventional search methods ineffective. As a result, the current best version of my agent utilizes Monte Carlo tree search with rollouts for the initial phase of the game, while employing a special [endgame search routine](https://daannoordenbos.github.io/Endgame-play-in-Amazons/) for the endgame phase.

## Snakes and Ladders
Snakes and Ladders, a classic children's board game played on a 10x10 grid with a die, is entirely governed by chance, making it a stochastic game. My [research](https://daannoordenbos.github.io/snakes-ladders/) focused on determining the win probabilities for the n-player generalization of the game using Markov chains.
