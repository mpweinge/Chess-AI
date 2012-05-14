#include <iostream>
#include "ChessPlayer.h"
#include "ChessBoard.h"
#include <vector>
#include "Constants.h"
#include <stdio.h>
#include "MoveTree.h"
#include "ChessChromosome.h"
#include "Tournament.h"
#include <fstream>

bool bGameOver;

int main (int argc, char * const argv[]) 
{
    // insert code here...
    //std::cout << "Hello, World!\n";
	
	bGameOver = false;
	
	WhitePlayer* whitePlayer = WhitePlayer::GetInstance();
	BlackPlayer* blackPlayer = BlackPlayer::GetInstance();
	
	bool bWhiteMove;
	bWhiteMove = true;
	
	int iLastPawnMove;
	iLastPawnMove = 0;
	
	Chromosome* chromo1 = new Chromosome(100);
	chromo1->LoadChromosome();
	Chromosome* chromo2 = new Chromosome(101);
	chromo2->LoadChromosome();
	
	MoveTree whiteTree(PLY, true, chromo1);
	MoveTree blackTree(PLY, false, chromo2);
	
	ChessBoard::GetInstance()->SetWhitePlayer(whitePlayer);
	ChessBoard::GetInstance()->SetBlackPlayer(blackPlayer);
	
	whitePlayer->SetPieceBoardPointer(ChessBoard::GetInstance());
	blackPlayer->SetPieceBoardPointer(ChessBoard::GetInstance());
	
	while ( !whitePlayer->IsCheckMated() && !blackPlayer->IsCheckMated() && !bGameOver)
	{
		if (iLastPawnMove == 50)
		{
			printf("50 moves without pawn moving");
			printf("Tie Game");
			break;
		}
		if(whitePlayer->GetNumPiecesLeft() == 1 ) 
		{
			if(blackPlayer->GetNumPiecesLeft() == 1) 
			{
				printf("Tie Game!");
				break;
			}
			else if (blackPlayer->GetNumPiecesLeft() == 2)
			{
				char p1 = pieceToText(blackPlayer->GetPiece(0));
				char p2 = pieceToText(blackPlayer->GetPiece(1));
				if ( p1 == 'B' || p2 == 'B' || p1 == 'N' || p2 == 'N' )
				{
					printf("Tie Game!");
					break;
				}
			}
		}
		else if(blackPlayer->GetNumPiecesLeft() == 1 ) 
		{
			if(whitePlayer->GetNumPiecesLeft() == 1) 
			{
				printf("Tie Game!");
				break;
			}
			else if (whitePlayer->GetNumPiecesLeft() == 2)
			{
				char p1 = pieceToText(whitePlayer->GetPiece(0));
				char p2 = pieceToText(whitePlayer->GetPiece(1));
				if ( p1 == 'B' || p2 == 'B' || p1 == 'N' || p2 == 'N' )
				{
					printf("Tie Game!");
					break;
				}
			}
		}
		
		printf("Press M for move or X to exit or directly enter in move");
		char* c = new char;
		std::cin >> c;
		
		int x; 
		int y;
		
		int destx;
		int desty;
		switch (*c)
		{
			case 'a':
			case 'A':
			{
				for (int i = 0; i < 1000; i++)
				{
					ChessBoard* ThreadBoard = ChessBoard::GetInstance()->Copy();
					ThreadBoard->Erase();
					delete (ThreadBoard);
				}
				break;
			}
			case 'Z':
			case 'z':
			{
				Tournament* newTour = new Tournament(4, 50, 4, 10);
				newTour->PlayTournament();
				break;
			}
			case 'm':
			case 'M':
			{
				//Lets play a match
				Chromosome* chromo1 = new Chromosome(106);
				chromo1->LoadChromosome();
				Chromosome* chromo2 = new Chromosome(107);
				chromo2->LoadChromosome();
				
				//Chromosome* ret = chromo1->Mutate((EMutateType)3, chromo2);
				//4 PLY, 50 moves
				Match* currMatch = Tournament::CreateMatch(chromo1, chromo2, 4, 50);
				currMatch->PlayMatch();
				
				delete (currMatch);
				delete(chromo1);
				delete(chromo2);
				bGameOver = true;
				break;
			}
			case 'f':
			case 'F':
			{
				bWhiteMove = !bWhiteMove;
				break;
			}
			case 'g':
			case 'G':
			{
				int k;
				k = 0;
				break;
			}
			case 'E':
			case 'e':
			{
				ChessBoard::GetInstance()->ResetBoard();
				bWhiteMove = true;
				break;
			}
			case 'C':
			case 'c':
			{
				srand((unsigned)time(0));

				for (int i = 0; i < 10; i++)
				{
					Chromosome* NewChromo = new Chromosome(i+100);
					//Mate the two parents
					for (int j = 0; j < NUM_WEIGHTINGS; j++)
					{
						int RandWeighting = rand() % 50;
						NewChromo->SetWeighting(j, RandWeighting);
					}
					NewChromo->SaveChromosome();
					delete(NewChromo);
				}
			}
				break;
			case 'p':
			case 'P':
				x = *(c+1);
				y = *(c+2);
				if( (x == 'B') || (x == 'b') && (y == '0'))
				{
				//	ChessBoard::GetInstance()->PrintBoard();
					break;
				}
				else if( (x == 'L') || (x == 'l')  && (y == '0'))
				{
					ChessLog::GetInstance()->printLog();
					break;
				}
				iLastPawnMove = -1;
			case 'n':
			case 'N':
			case 'b':
			case 'B':
			case 'q':
			case 'Q':
			case 'k':
			case 'K':
			case 'r':
			case 'R':
			{
				iLastPawnMove++;
				//Person wants to move piece
				x = *(c+1);
				y = *(c+2);
				
				destx = *(c+4);
				desty = *(c+5);
				
				//X should be x aka a,b,c,d,e etc
				//Ascii 'a' = 97
				//Ascii 'A' = 65
				if ( (x - 65) < 8 && x > 64)
				{
					x -= 65;
				}
				else if ( (x - 97) < 8 && x > 96)
				{
					x -= 97;
				}
				else {
					printf("Invalid Input");
					break;
				}				
				//Ascii 1 = 49
				if ( (y - 49) < 8 && y > 48)
					y -= 49;
				else
				{
					printf("Invalid Input");
					break;
				}
				
				//Y should be row, aka 1-8
				//These are stored as ascii chars
				
				if ( (destx - 65) < 8 && destx > 64)
				{
					destx -= 65;
				}
				else if ( (destx - 97) < 8 && destx > 96)
				{
					destx -= 97;
				}
				else {
					printf("Invalid Input");
					break;
				}				
				//Ascii 1 = 49
				if ( (desty - 49) < 8 && desty > 48)
					desty -= 49;
				else
				{
					printf("Invalid Input");
					break;
				}
				
				//Must be valid
				if (! (ChessBoard::GetInstance()->bHasPiece(x,y)) )
				{
					printf("No piece at %c%i", *(c+1), y+1);
					break;
				}
				
				ChessPiece* piece = ChessBoard::GetInstance()->GetPiece(x,y);
				
				if ( piece->isWhite() != bWhiteMove )
				{
					printf("Wrong colour");
					break;
				}
				
				if (piece)
				{
					Move move2 = Move(destx,desty);
					if (!(piece->MakeMove(move2)))
					{
						printf("Invalid move, buster");
						break;
					}
					else {
						//assert(0);
						//blackTree.PruneBranches();
						//whiteTree.PruneBranches();
					}

				}
				
				Player* currPlayer;
				if (bWhiteMove)
					currPlayer = WhitePlayer::GetInstance();
				else
					currPlayer = BlackPlayer::GetInstance();
				
				for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
				{
					//char name = pieceToText(currPlayer->GetPiece(i));
					//if( (name == 'R') || (name == 'Q') || (name == 'B') )
					{
						if ( currPlayer->GetPiece(i)->LookForCheck(true) )
						{
							printf("Check!");
							if (bWhiteMove)
							{
								BlackPlayer::GetInstance()->EnterCheckState();
								break;
							}
							else {
								WhitePlayer::GetInstance()->EnterCheckState();
								break;
							}

							
						}
					}
				}
				
				bWhiteMove = !bWhiteMove;
				
				//blackTree.GenerateTree();
				
				break;
			}
				
			case 'o':
			case 'O':
			{
#ifdef SUPPRESS_OUTPUT
#undef SUPPRESS_OUTPUT
#endif
				int dash = *(c+2);
				if( (dash != 'O') && (dash != 'o') )
				{
					printf("Invalid input");
					break;
				}
				char dash2 = *(c+4);
				//Check for castling
				
				if (bWhiteMove)
				{
					int col = 0;
					if( (dash2 == 'o') || (dash2 =='0') || (dash2 == 'O') )
					{
						if (WhitePlayer::GetInstance()->CanCastle(false, ChessBoard::GetInstance()) )
						{
							bWhiteMove = !bWhiteMove;
							ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
							ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(0, col);
							ChessBoard::GetInstance()->Castle(_King, _Rook, false, col);
							//assert(0);
							//blackTree.PruneBranches();
							//whiteTree.PruneBranches();
						}
					}
					else {
						if (WhitePlayer::GetInstance()->CanCastle(true, ChessBoard::GetInstance()) )
						{
							bWhiteMove = !bWhiteMove;
							ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
							ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(7, col);
							ChessBoard::GetInstance()->Castle(_King, _Rook, true, col);
							//assert(0);
							//blackTree.PruneBranches();
							//whiteTree.PruneBranches();
						}
					}

				}
				else { 
					int col = 7;
					if( (dash2 == 'o') || (dash2 =='0') || (dash2 == 'O') )
					{
						if (BlackPlayer::GetInstance()->CanCastle(false, ChessBoard::GetInstance()) )
						{
							bWhiteMove = !bWhiteMove;
							ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
							ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(0, col);
							ChessBoard::GetInstance()->Castle(_King, _Rook, false, col);
							ChessAssert();
							blackTree.PruneBranches();
							whiteTree.PruneBranches();
						}
					}
					else {
						if (BlackPlayer::GetInstance()->CanCastle(true, ChessBoard::GetInstance()) )
						{
							bWhiteMove = !bWhiteMove;
							ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
							ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(7, col);
							ChessBoard::GetInstance()->Castle(_King, _Rook, true, col);
							ChessAssert();
							blackTree.PruneBranches();
							whiteTree.PruneBranches();
						}
					}
				}

#define SUPPRESS_OUTPUT
					
				
				break;
			}
				
			case 't':
			case 'T':
				if (bWhiteMove)
				{
					//ChessAssert();
					whiteTree.GenerateTree();
					blackTree.PruneBranches();
				}
				else
				{
					//ChessAssert();
					blackTree.GenerateTree();
					whiteTree.PruneBranches();
				}
				bWhiteMove = !bWhiteMove;
				//blackTree.PrintTree();
				break;
			case 'x':
			case 'X':
				bGameOver = true;
				break;
			case 'd':
			case 'D':
			{
				std::vector<ChessMove> potMoves;
				Player* Player;
				x = *(c+1);
				y = *(c+2);
				if( (x == 'w') || (x == 'W') )
					Player = WhitePlayer::GetInstance();
				else {
					Player = BlackPlayer::GetInstance();
				}

				Move newPieceLoc;
				ChessPiece* currPiece;
				
				for(int i = 0; i < Player->GetNumPiecesLeft(); i++)
				{
					if (y == 0)
					{
						currPiece = Player->GetPiece(i);
						newPieceLoc = currPiece->GetLocation();
						//if (i > 1)
							currPiece->GetLegalMoves(potMoves);
						for (int i = 0; i < potMoves.size(); i++)
						{
							ChessMove& currMove = potMoves[i];
							if (currMove.isPromotion() )
							{
								printf("Piece:%c%c%i-%c%i%c\n", pieceToText(currPiece), rowToLetter(newPieceLoc.row), newPieceLoc.col+1,
									   rowToLetter(currMove.getRow()), currMove.getCol()+1, currMove.GetPromotionChar());
							}
							else if (currMove.isCastle() )
							{
								if (currMove.isKingSideCastle() )
									printf("O-O\n");
								else {
									printf("O-O-O\n");
								}

							}
							else if ( currMove.IsEnPassant() )
							{
								printf("P%c%ix%c%i", rowToLetter(currMove.getStartingRow()), currMove.getStartingCol()+1, rowToLetter(currMove.getRow()), currMove.getCol()+1);
							}
							else
							{
								printf("Piece:%c%c%i-%c%i\n", pieceToText(currPiece), rowToLetter(newPieceLoc.row), newPieceLoc.col+1,
									   rowToLetter(currMove.getRow()), currMove.getCol()+1);
							}
							fflush(stdout);
						}
					}
					else 
					{
						currPiece = Player->GetPiece(i);
						if (pieceToText(currPiece) == y)
						{
							newPieceLoc = currPiece->GetLocation();
							//if (i > 1)
							currPiece->GetLegalMoves(potMoves);
							for (int i = 0; i < potMoves.size(); i++)
							{
								ChessMove& currMove = potMoves[i];
								if (currMove.isPromotion() )
								{
									printf("Piece:%c%c%i-%c%i%c\n", pieceToText(currPiece), rowToLetter(newPieceLoc.row), newPieceLoc.col+1,
										   rowToLetter(currMove.getRow()), currMove.getCol()+1, currMove.GetPromotionChar());
								}
								else if (currMove.isCastle() )
								{
									if (currMove.isKingSideCastle() )
										printf("O-O\n");
									else {
										printf("O-O-O\n");
									}
									
								}
								else if (currMove.IsEnPassant() )
								{
									printf("P%c%ix%c%i", rowToLetter(currMove.getStartingRow()), currMove.getStartingCol()+1, rowToLetter(currMove.getRow()), currMove.getCol()+1);
								}
								else
								{
									printf("Piece:%c%c%i-%c%i\n", pieceToText(currPiece), rowToLetter(newPieceLoc.row), newPieceLoc.col+1,
										   rowToLetter(potMoves[i].getRow()), potMoves[i].getCol()+1);
								}
								fflush(stdout);
							}
						}
					}
					potMoves.clear();
				}
				break;
			}
			case 'U':
			case 'u':
				ChessBoard::GetInstance()->UndoMove();
				bWhiteMove = !bWhiteMove;
				break;
			default:
				//printf(" %s\n", c);
				//ChessBoard::GetInstance()->PrintBoard();
				break;
		}
		ChessLog::GetInstance()->printLog();
		ChessBoard::GetInstance()->PrintBoard();
		delete(c);
	}
	delete(chromo1);
	delete(chromo2);
	ChessBoard::GetInstance()->Erase();
	
    return 0;
}
