# NP

C++ Candy Crush Game on sever

Client able to play the game

<br/><br/>

## ZMQ messages for client walktrough:

* Start: Candy>start?> and Candy>start!>

* Get grid: Candy>grid?> and Candy>grid!>

* Get dimentions for grid: Candy>x!> and Candy>y!>

* Ask if ready to play: Candy>play!>

* Get Colmum for move: Candy>colum?> and Candy>colum!>

* Get Row for move: Candy>row?> and Candy>row!>

* Make a move: Candy>move?> and Candy>move!>

* Confirm if move is legal: Candy>legal!

<br/><br/>

### Start
Server listening on Candy>start?> and answers with Candy>start!> to cofirm start of the game
### Grid Dimentions
Server is listening to Candy>x!> to get x and Candy>y!> to get y and makes a grid
### Print Grid
Client sends Candy>grid?> and starts listening to Candy>grid!> to print the grid
### Play
Server listening to Candy>play!> for comfirmation
### Colum and Row
Server sends Candy>colum?> or Candy>Row?> and starts listening to Candy>colum!> or Candy>Row!> to prepare for the move
### Move
Server sends Candy>move?> and starts listening to Candy>move!> and makes the move
### Legal
Server listening to Candy>legal!> to confirm if the move is legal

<br/><br/>

## Communication
The client and server communicate via the broker. Both are subscribed on (tcp://benternet.pxl-ea-ict.be:24042) en push to (tcp://benternet.pxl-ea-ict.be:24041).
When you want to ask something you use ?> and when you answer you use !>. Example: Candy>row?> -->  Candy>row!>8
Both the client and the server push to the pull port of the broker and subscribe on the publishing port of the broker.
