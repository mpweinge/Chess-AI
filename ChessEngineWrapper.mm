//
//  ChessEngineWrapper.m
//  Sudoku
//
//  Created by Michael Weingert on 12-05-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "ChessEngineWrapper.h"
#include "./ChessEngine/ChessBoard.h"
#include "./ChessEngine/MoveTree.h"
#include "MyOpenGLView.h"
#include "Constants.h"
class ChessSquare;

/*class Square
{
	int row;
	int col;
};*/

struct CocoaThreadStruct
{
	MoveTree* Tree;
};

class CImplement
{
private:
	ChessPiece* HoldingPiece;
	bool bWhiteMove;
	Chromosome* treeChromo;
	NSTextFieldCell* LocalMoveList;
	MyOpenGLView* OpenGLView;
	bool PlayAgainstCPU;
public:
	void SetMoveList(NSTextFieldCell* _IncomingMoveList)
	{
		LocalMoveList = _IncomingMoveList;
	}
	CImplement(void)
	{
		ChessBoard::GetInstance()->SetWhitePlayer(WhitePlayer::GetInstance());
		ChessBoard::GetInstance()->SetBlackPlayer(BlackPlayer::GetInstance());
		ChessBoard::GetInstance()->ResetBoard();
		[[PieceQueue getInstance ]ResetQueue];
		UpdateBoard();
		//bWhiteMove = true;
		PlayAgainstCPU = false;
		HoldingPiece = NULL;
		treeChromo = new Chromosome(0);
		treeChromo->LoadChromosome();
	}
	
	void GenerateTree( int PLY)
	{
		if (ChessBoard::GetInstance()->IsWhiteMove())
		{
			if (WhitePlayer::GetInstance()->IsCheckMated() )
			{
			}
			else 
			{
				Chromosome* tempChromo = new Chromosome(4909);
				tempChromo->LoadChromosome();
				MoveTree CurrTree(PLY, true, tempChromo);
				CurrTree.GenerateTree();
			}
		}
		else {
			if (WhitePlayer::GetInstance()->IsCheckMated() )
			{
				
			}
			else 
			{
				Chromosome* tempChromo = new Chromosome(4909);
				tempChromo->LoadChromosome();
				MoveTree CurrTree(PLY, false, tempChromo);
				CurrTree.GenerateTree();
			}
		}
		//bWhiteMove = !bWhiteMove;
		[[PieceQueue getInstance ]ResetQueue];
		UpdateBoard();
	}
	
	void GetLegalMoves(int x, int y)
	{
		std::vector<ChessMove> Moves;
		ChessPiece* currPiece = ChessBoard::GetInstance()->GetPiece(x,y);
		[[PieceQueue getInstance ]ResetQueue];
		UpdateBoard();
		if (currPiece == NULL)
			return;
		if (HoldingPiece)
		{
			if (currPiece->isWhite() == ChessBoard::GetInstance()->IsWhiteMove())
				HoldingPiece = currPiece;
			else
				return;;
		}
		else {
			HoldingPiece = currPiece;
		}

		HoldingPiece->GetLegalMoves(Moves);
		for (int i = 0; i < Moves.size(); i++)
		{
			//Get queue and add squares to queue
			if (Moves[i].isCastle() )
			{
				//Do nothing for now
				int row = Moves[i].isKingSideCastle() ? 6 : 1;
				int col = ChessBoard::GetInstance()->IsWhiteMove()? 0 : 7;
				[[SquareDrawQueue getInstance]AddItemToQueue:row : col ];
			}
			else if (Moves[i].IsEnPassant() )
			{
				int row = Moves[i].getRow();
				int col = HoldingPiece->isWhite()? HoldingPiece->GetCol() + 1 : HoldingPiece->GetCol() -1;
				[[SquareDrawQueue getInstance]AddItemToQueue:row : col ];
			}
			else 
			{
				[[SquareDrawQueue getInstance]AddItemToQueue:Moves[i].getRow(): Moves[i].getCol() ];
			}

		}
	}
	
	char GetPieceChar(int row, int col)
	{
		ChessPiece* piece = ChessBoard::GetInstance()->GetPiece(row, col);
		if (piece)
			return pieceToText(piece);
		else {
			return ' ';
		}

	}
	
	void TestLegalMove(int row, int col)
	{
		if (HoldingPiece == NULL)
			return;
		if (HoldingPiece->GetRow() == row && HoldingPiece->GetCol() == col)
			return;
		
		if ( HoldingPiece->isWhite() != ChessBoard::GetInstance()->IsWhiteMove() )
		{
			printf("Wrong colour");
			return;
		}
		bool bBreakingOut = false;
		//if (HoldingPiece)
		{
			ChessSquare move2 = ChessSquare(row,col);

			if (pieceToText(HoldingPiece) == 'K')
			{
				//Check for castle here
				if (abs(move2.row - HoldingPiece->GetRow()) > 1)
				{
						//Check for castling
						int col = 0;
						if (ChessBoard::GetInstance()->IsWhiteMove())
						{
							if(move2.row == 0)
							{
								if (WhitePlayer::GetInstance()->CanCastle(false, ChessBoard::GetInstance()) )
								{
									ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
									ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(0, col);
									ChessBoard::GetInstance()->Castle(_King, _Rook, false, col);
									bBreakingOut = true;
								}
							}
							else 
							{
								if (WhitePlayer::GetInstance()->CanCastle(true, ChessBoard::GetInstance()) )
								{
									ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
									ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(7, col);
									ChessBoard::GetInstance()->Castle(_King, _Rook, true, col);
									bBreakingOut = true;
								}
							}
							
						}
						else { 
							int col = 7;
							if (move2.row == 0)
							{
								
								if (BlackPlayer::GetInstance()->CanCastle(false, ChessBoard::GetInstance()) )
								{
									ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
									ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(0, col);
									ChessBoard::GetInstance()->Castle(_King, _Rook, false, col);
									bBreakingOut = true;
									if (PlayAgainstCPU)
									{
										[[ChessEngineWrapper getInstance]GenerateTree];
									}
								}
							}
							else {
								if (BlackPlayer::GetInstance()->CanCastle(true, ChessBoard::GetInstance()) )
								{
									ChessPiece* _King = ChessBoard::GetInstance()->GetPiece(4, col);
									ChessPiece* _Rook = ChessBoard::GetInstance()->GetPiece(7, col);
									ChessBoard::GetInstance()->Castle(_King, _Rook, true, col);
									bBreakingOut = true;
									if (PlayAgainstCPU)
									{
										[[ChessEngineWrapper getInstance]GenerateTree];
									}
								}
							}
						}
					}
				}
			if (!(HoldingPiece->MakeMove(move2)) && !bBreakingOut)
			{
				printf("Invalid move, buster");
				return;
			}
			else {
				
			}
			
		}
		
		Player* currPlayer;
		if (ChessBoard::GetInstance()->IsWhiteMove())
			currPlayer = WhitePlayer::GetInstance();
		else
			currPlayer = BlackPlayer::GetInstance();
		if (!bBreakingOut)
		{
			for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
			{
				//char name = pieceToText(currPlayer->GetPiece(i));
				//if( (name == 'R') || (name == 'Q') || (name == 'B') )
				{
					if ( currPlayer->GetPiece(i)->LookForCheck(true) )
					{
						printf("Check!");
						if (ChessBoard::GetInstance()->IsWhiteMove())
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
			[[ChessEngineWrapper getInstance]UpdateMoveList];
			//bWhiteMove = !bWhiteMove;
			
			if (PlayAgainstCPU)
			{
				[[ChessEngineWrapper getInstance]GenerateTree];
			}
			else {
				
			}

		}
		
	}
	void UpdateImplementBoard()
	{
		UpdateBoard();
	}
	void UpdateMoveList()
	{
		NSString* currContents = @"White    |Black    \n";
		
		//= [ObjectiveCObject stringValue];
		for (int i = 0; i < ChessLog::GetInstance()->GetNumItemsInLog(); i++)
		{
			
			NSString* MoveString = [NSString stringWithFormat:@"%@", 
											  ChessMoveToNSString(ChessLog::GetInstance()->getIndexedItem(i))];
			if (!(i&1))
				currContents = [currContents stringByAppendingString: @"\n"];
			currContents = [currContents stringByAppendingString: MoveString];
		}
		//[ObjectiveCObject setStringValue:@"Really Big Text"];
		//NSString* currContents = [ObjectiveCObject stringValue];
		[LocalMoveList setStringValue:currContents];
	}
	
	NSString* ChessMoveToNSString(const ChessMove* CurrMove)
	{
		char outText;
		if (CurrMove->isCastle())
		{
			if ( CurrMove->isKingSideCastle() )
			{
				return [NSString stringWithFormat:@"O-O      |"];
			}
			else {
				return [NSString stringWithFormat:@"O-O-O    |"];
			}
			//continue;
		}
		
		
		pieceToText(CurrMove->getPiece(), outText);
		
		char CheckMove = ' ';
		
		if( CurrMove->IsCheckMove() )
		{
			CheckMove = '+';
		}
		
		if ( !(CurrMove->isCapture() ) )
		{
			NSString* tempString = [NSString stringWithFormat:@"%c%c%i-%c%i%c", outText, rowToLetter(CurrMove->getStartingRow()), CurrMove->getStartingCol()+1,
				   rowToLetter(CurrMove->getRow()), CurrMove->getCol()+1, CheckMove];
			if (CurrMove->isPromotion())
			{
				return [tempString stringByAppendingFormat:@"%c |", CurrMove->GetPromotionChar() ];
				//printf("%c|", CurrMove->GetPromotionChar() );
			}
			else {
				return [tempString stringByAppendingFormat:@"  |"];
				//printf("|");
			}
			
		}
		else {
			char outText2;
			pieceToText(CurrMove->getCapturedPiece() , outText2);
			NSString* tempString = [NSString stringWithFormat:@"%c%c%ix%c%c%i%c", outText, rowToLetter(CurrMove->getStartingRow()), CurrMove->getStartingCol()+1,
				   outText2, rowToLetter(CurrMove->getRow()), CurrMove->getCol()+1, CheckMove];
			if (CurrMove->isPromotion())
			{
				return [tempString stringByAppendingFormat:@"%c|", CurrMove->GetPromotionChar() ];
				//printf("%c|", CurrMove->GetPromotionChar() );
			}
			else {
				return [tempString stringByAppendingFormat:@" |"];
				//printf("|");
			}
		}
	}
	
	static void* PosixThreadMainRoutine(void* data)
	{
		// Do some work here.
		((CocoaThreadStruct*)data)->Tree->GenerateTree();
		
		return NULL;
	}
	
	void PlayMatch()
	{
		ChessBoard::GetInstance()->PrintBoard();
		Chromosome* Competitor1 = new Chromosome(ChromosomeNumber1);
		Chromosome* Competitor2 = new Chromosome(ChromosomeNumber2);
		
		int MoveLimit = 100;
		int PLY = 4;
		
		Competitor1->LoadChromosome();
		Competitor2->LoadChromosome();
		
		MoveTree* WhiteTree = new MoveTree(PLY, true, Competitor1);
		MoveTree* BlackTree = new MoveTree(PLY, false, Competitor2);
		int i;
		bool bTie = false;
		
		std::vector<ChessBoard*> PastBoards;
		PastBoards.reserve(100);
		
		int RepeatBoards = 0;
		
		for (i = 0; i <MoveLimit; i++)
		{
			if (ChessBoard::GetInstance()->IsWhiteMove())
			{
				pthread_t       posixThreadID;
				struct CocoaThreadStruct pThreadStruct;
				pThreadStruct.Tree = WhiteTree;
				int     threadError = pthread_create(&posixThreadID, NULL, &PosixThreadMainRoutine, &pThreadStruct);
			
				pthread_join(posixThreadID, NULL);
				
				//WhiteTree->GenerateTree();
				BlackTree->PruneBranches();
				[[PieceQueue getInstance]ResetQueue];
				UpdateBoard();
				[[ChessEngineWrapper getInstance] SelectorMethod ];
				//UpdateMoveList();
				//[OpenGLView setNeedsDisplay:true];
				for (int k = PastBoards.size() - 1; k >= 0; k--)
				{
					if (ChessBoard::GetInstance()->IsEqual(PastBoards[k]))
					{
						RepeatBoards++;
						if (RepeatBoards > 1)
						{
							bTie = true;
							i = MoveLimit;
							break;
						}
					}
				}
				RepeatBoards = 0;
				if (bTie)
					break;
				else 
				{
					PastBoards.push_back(ChessBoard::GetInstance()->Copy() );
				}
			
				
				if (BlackPlayer::GetInstance()->IsInCheck())
				{
					if (BlackPlayer::GetInstance()->CheckForCheckMate() )
					{
						//Winner = Competitor1;
						//Loser = Competitor2;
						break;
					}
				}
				else if (BlackPlayer::GetInstance()->CheckForStalemate() )
				{
					i = MoveLimit;
					bTie = true;
					break;
				}
			
				ChessLog::GetInstance()->printLog();
				ChessBoard::GetInstance()->PrintBoard();
				//bWhiteMove = !bWhiteMove;
			}
			pthread_t       posixThreadID;
			struct CocoaThreadStruct pThreadStruct;
			pThreadStruct.Tree = BlackTree;
			int threadError = pthread_create(&posixThreadID, NULL, &PosixThreadMainRoutine, &pThreadStruct);
			
			pthread_join(posixThreadID, NULL);
			
			//BlackTree->GenerateTree();
			WhiteTree->PruneBranches();
			[[PieceQueue getInstance]ResetQueue];
			UpdateBoard();
			[[ChessEngineWrapper getInstance] SelectorMethod ];
			 //UpdateMoveList();
			//[OpenGLView setNeedsDisplay:true];
			
			for (int k = 0; k < PastBoards.size(); k++)
			{
				if (ChessBoard::GetInstance()->IsEqual(PastBoards[k]))
				{
					RepeatBoards++;
					if (RepeatBoards > 1)
					{
						bTie = true;
						i = MoveLimit;
						break;
					}
				}
			}
			RepeatBoards = 0;
			if (bTie)
				break;
			else 
			{
				PastBoards.push_back(ChessBoard::GetInstance()->Copy() );
			}
			
			if (WhitePlayer::GetInstance()->IsInCheck() )
			{
				if (WhitePlayer::GetInstance()->CheckForCheckMate() )
				{
					//Winner = Competitor2;
					//Loser = Competitor1;
					break;
				}
			}
			else if (WhitePlayer::GetInstance()->CheckForStalemate() )
			{
				i = MoveLimit;
				bTie = true;
				break;
			}
			
			ChessLog::GetInstance()->printLog();
			ChessBoard::GetInstance()->PrintBoard();
			//bWhiteMove = !bWhiteMove;
		}
		//printf("Game over, final position below: Tie? %c\n", bTie? 'y' : 'n');
		//fflush(stdout);
		//ChessLog::GetInstance()->printLog();
		//ChessBoard::GetInstance()->PrintBoard();
		
		for (int i = 0; i < PastBoards.size(); i++)
		{
			PastBoards[i]->Erase();
			delete(PastBoards[i]);
		}
		PastBoards.clear();
		
		delete(WhiteTree);
		delete(BlackTree);
		
		//HoldingPiece = NULL;
	}
	void ResetBoard()
	{
		ChessBoard::GetInstance()->ResetBoard();
		HoldingPiece = NULL;
	}
	void SetView(MyOpenGLView* _OpenGLView)
	{
		OpenGLView = _OpenGLView;
	}
	void SetPlay(bool _Play)
	{
		PlayAgainstCPU = _Play;
	}
};

void UpdateBoard()
{
	WhitePlayer* whitePlayer = WhitePlayer::GetInstance();
	BlackPlayer* blackPlayer = BlackPlayer::GetInstance();
	for (int i = 0; i < whitePlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* _ChessPiece = whitePlayer->GetPiece(i);
		GLChessPiece* currPiece = [[GLChessPiece alloc]initWithParameters:_ChessPiece->GetRow(): _ChessPiece->GetCol(): pieceToText(_ChessPiece): true];
		[[PieceQueue getInstance]AddPieceToQueue: currPiece];
	}
	for (int i = 0; i < blackPlayer->GetNumPiecesLeft(); i++)
	{
		ChessPiece* _ChessPiece = blackPlayer->GetPiece(i);
		GLChessPiece* currPiece = [[GLChessPiece alloc]initWithParameters:_ChessPiece->GetRow(): _ChessPiece->GetCol(): pieceToText(_ChessPiece): false];
		[[PieceQueue getInstance]AddPieceToQueue: currPiece];
	}
}


@implementation ChessEngineWrapper

static ChessEngineWrapper* m_Instance;

- (id)init
{
	[super init];
	mImplement = new CImplement;
	PlayAgainstCPU = false;
	return self;
}

+ (ChessEngineWrapper *) getInstance
{
	if (m_Instance == NULL)
		m_Instance = [[self alloc] init];
	
	return m_Instance;
}

- (void)GetLegalLocations: (int) x: (int) y
{
	mImplement->GetLegalMoves(x, y);
}

- (void)TestLegalMove:(int)x :(int)y
{
	mImplement->TestLegalMove(x,y);
}

- (void)UpdateBoard
{
	mImplement->UpdateImplementBoard();
}

void* PosixThreadMainRoutine3(void* data)
{
	((CocoaMatchStruct*)data)->mImplement->GenerateTree(4);
	[[ChessEngineWrapper getInstance] UpdateMoveList];
	[[ChessEngineWrapper getInstance] UpdateButtons];
	[[ChessEngineWrapper getInstance] UpdateGL];
}

- (void)GenerateTree
{
	[GenerateMoveButton setEnabled:FALSE];
	[PlayMatchButton setEnabled:FALSE];
	[PlayAgainstCPUButton setEnabled:FALSE];
	pthread_t       posixThreadID;
	struct CocoaMatchStruct pThreadStruct;
	pThreadStruct.mImplement = mImplement;
	int     threadError = pthread_create(&posixThreadID, NULL, &PosixThreadMainRoutine3, &pThreadStruct);
	
}

-(void)UpdateMoveList
{
	mImplement->UpdateMoveList();
}

-(void)UpdateGL
{
	[self UpdateMoveList];
	[OpenGLView setNeedsDisplay:YES];
}

-(void)UpdateButtons
{
	[GenerateMoveButton setEnabled:YES];
	[PlayMatchButton setEnabled:YES];
	[PlayAgainstCPUButton setEnabled:YES];
}

- (void)SetMoveList : (NSTextFieldCell*) MoveList
{
	LocalMoveList = MoveList;
	mImplement->SetMoveList(LocalMoveList);
}

void* PosixThreadMainRoutine2(void* data)
{
	((CocoaMatchStruct*)data)->mImplement->PlayMatch();
}

-(void)PlayMatch
{
	[GenerateMoveButton setEnabled:FALSE];
	[PlayAgainstCPUButton setEnabled:FALSE];
	[PlayMatchButton setEnabled:FALSE];
	pthread_t       posixThreadID;
	struct CocoaMatchStruct pThreadStruct;
	pThreadStruct.mImplement = mImplement;
	int     threadError = pthread_create(&posixThreadID, NULL, &PosixThreadMainRoutine2, &pThreadStruct);
	
    //returnVal = pthread_attr_destroy(&attr);
    //assert(!returnVal);*/
    if (threadError != 0)
    {
		// Report an error.
    }
	
	//mImplement->PlayMatch();
}

-(void)ResetBoard
{
	mImplement->ResetBoard();
	[self UpdateMoveList];
	[self UpdateButtons];
}

-(void)SetOpenGLView : (MyOpenGLView*) _OpenGLView
{
	OpenGLView = _OpenGLView;
	mImplement->SetView(OpenGLView);
}
-(void)SelectorMethod
{
	[self performSelectorOnMainThread:@selector(UpdateGL) withObject:nil waitUntilDone:NO];
}

-(void)OtherSelectorMeth0d
{
	[self performSelectorOnMainThread:@selector(UpdateButtons) withObject:nil waitUntilDone:NO];
}

- (void)SetButtons: (NSButtonCell*)_GenerateMoveButton : (NSButtonCell*)_PlayMatchButton : (NSButtonCell*) _PlayAgainstCPUButton
{
	GenerateMoveButton = _GenerateMoveButton;
	PlayMatchButton = _PlayMatchButton;
	PlayAgainstCPUButton = _PlayAgainstCPUButton;
}

- (void)SetPlayAgainstCPU: (bool)_Play
{
	PlayAgainstCPU = _Play;
	mImplement->SetPlay(_Play);
}
@end
