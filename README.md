# ED Game
An engine for a deck-building game based on Eminent Domain
### Goals
The purpose of this project is to implement a game engine for the popular deck-building card game, Eminent Domain by Seth Jaffee (see https://boardgamegeek.com/boardgame/68425/eminent-domain). Doing so of course places this project in a potentially questionable legal ground were it to be sold, so for that reason it is worth noting that this is _not_ intended as a standalone digital product that replaces the game. What this project is is an implementation of the rules engine and state management, which
 - Cannot be played without access to a copy of the game's rulebook
 - Uses none of the art and (maybe?) restricted use of proper names
 - Is not meant to be monitized
Now for the more positive features of what this project _is_ for:
 - A framework for developing algorithmic strategies for the game
 - An extensible framework for making edits to the base game
 - A way for people who are not collocated to play the game
 - A way of playing the game without immense setup
 - A fun side project for me to pass the time developing

The core engine of the game I hope to design to be general enough that it will work either on a single computer using a text interface or with ai players, but also could be run as a rules server with some (potentially graphical) separate user interface presented to players.

At this point, the project is very much under construction, and there are no planned deadlines. I am still at the "write fragmented ideas" as code phase, so don't expect to be able to take the code in the current state and actually do anything with it.

(c) Sam Donow 2017
I have copyright of all original text and source code in this repository, but the copyright of Eminent Domain and associated copyrights belong to Seth Jaffee and Tasty Minstrel Games.
