/*
 *  MoveTree.cpp
 *  ChessEngine
 *
 *  Created by Michael Weingert on 12-02-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MoveTree.h"

static void* ThreadStartFunction(void* _pThreadData)
{
	ChessBoard* ThreadBoard = ChessBoard::GetInstance()->Copy();
	//printf("Threadboard address %i\n", ThreadBoard);
	//fflush(stdout);
	//Need a copy of chessboard,
	//ThreadBoard->PrintBoard();
	struct ThreadStruct* pThreadData = (struct ThreadStruct *)_pThreadData;
	
	MoveNode* pThreadCopy = pThreadData->CurrentNode->Copy();
	
	pThreadCopy->UnThreadedCopy = pThreadData->CurrentNode;
	
	bool bIsCastleMove = pThreadCopy->GetMove().isCastle();
	
	ChessPiece* tempPiece = NULL;
	
	if (bIsCastleMove)
	{
		
	}
	else
	{
		bool bIsWhite = pThreadCopy->GetMove().getPiece()->isWhite();
		tempPiece = bIsWhite ? 
		ThreadBoard->GetWhitePlayer()->FindPiece(pThreadCopy->GetMove().getPiece() ) :
		ThreadBoard->GetBlackPlayer()->FindPiece(pThreadCopy->GetMove().getPiece() );
	
		pThreadCopy->GetMove().setPiece(tempPiece);
	}
	
	pThreadCopy->SetChessBoardPtr(ThreadBoard);
	
	//ThreadBoard->PrintBoard();
	
	pThreadData->Tree->FillAlphaBetaNode(pThreadData->Ply, pThreadCopy, !pThreadData->bIsWhite, ThreadBoard->GetWhitePlayer(), ThreadBoard->GetBlackPlayer(), ThreadBoard );
	
	ThreadBoard->Erase();
	delete(ThreadBoard);
	pThreadCopy->Erase();
	delete (pThreadCopy);
	
	pthread_exit(NULL);
	//Clean up here
	//return NULL;
}

MoveTree::MoveTree(int Ply, bool _IsWhite, Chromosome* _AttachedChromosome)
{
	FixedPly = Ply;
	assert((Ply & 1) == 0);
	bIsWhite = _IsWhite;
	/*for (int i = 0; i <= Ply; i++)
	{
		TreeLevels.push_back(new TreeLevel(i&1 == 1));
	}*/
	//attachedChromosome = new Chromosome(ChromosomeNum);
	//attachedChromosome->LoadChromosome();
	attachedChromosome = _AttachedChromosome;
	
	if (attachedChromosome)
	{
		assert(attachedChromosome->IsLoaded() );
		root = new MoveNode(ChessMove(ChessBoard::GetInstance()), NULL, false, bIsWhite, attachedChromosome, ChessBoard::GetInstance());
		root->depth = 0;
	}
	m_iCurrentRunningThreads = 0;
}

void MoveTree::GenerateTree()
{
	//Player* currPlayer;
	/*if (bIsWhite)
		currPlayer = WhitePlayer::GetInstance();
	else {
		currPlayer = BlackPlayer::GetInstance();
	}*/

	//ChessPiece* currPiece;
	//Move currLocation;
	
	//std::vector<Move*> LegalMoves;
	
	//MoveNode* currParent = root;
	
	//First ply
	
	//Player* whitePlayer = WhitePlayer::GetInstance();
	//Player* blackPlayer = BlackPlayer::GetInstance();
	if (root)
	{
		//root->Erase();
		//delete (root);
		root->depth = 0;
	}
	else {
		root = new MoveNode(ChessMove(ChessBoard::GetInstance()), NULL, false, bIsWhite, attachedChromosome, ChessBoard::GetInstance());
		root->depth = 0;
	}

	if (bIsWhite == root->IsMaxLevel() )
		root->SwitchMinLevel();
	
	root->SetBestMove(NULL);
	root->SetMinScore(0x7FFFFFFF);
	root->SetMaxScore(-10000000);
	m_iCurrentRunningThreads = 0;
	FillAlphaBetaNode(FixedPly, root, bIsWhite, WhitePlayer::GetInstance(), BlackPlayer::GetInstance(), ChessBoard::GetInstance());
	//FillNode(FixedPly, root, bIsWhite);
	//FindScore(root, bIsWhite);
	fflush(stdout);
	
	//root->PrintNode();
	//printf("Best Move: " );
	/*for (int i = 0; i < root->GetNumChildren(); i++)
	{
		root->GetChild(i)->PrintNode();
	}*/
	//
	/*for (int i = 0; i < root->GetNumChildren(); i++)
	{1
		root->GetChild(i)->PrintNode();
	}*/
	//BestMove->PrintNode();
	//printf("Best Move: \n" );
	/*assert(TreeLevels[1]->GetBestMove() );
	TreeLevels[1]->GetBestMove()->PrintNode();
	TreeLevels[1]->GetBestMove()->MakeMove();*/
	//root->GetBestMove()->PrintNode();
	
	int LocalMax = -230000000;
	int LocalMin = 0x7FFFFFFF;
	
	for (int i = 0; i < root->GetNumChildren(); i++)
	{
		if ( root->IsMaxLevel() )
		{
			if (root->GetChild(i)->GetBoardScore() < LocalMin)
			{
				root->SetBestMove(root->GetChild(i));
				LocalMin = root->GetChild(i)->GetBoardScore();
			}
		}
		else 
		{
			if (root->GetChild(i)->GetBoardScore() > LocalMax)
			{
				root->SetBestMove(root->GetChild(i));
				LocalMax = root->GetChild(i)->GetBoardScore();
			}
		}

	}
	if (root->GetNumChildren() == 0)
		return;
	assert(root->GetBestMove());
	root->GetBestMove()->SetChessBoardPtr(ChessBoard::GetInstance());
	
	if (root->GetBestMove()->GetMove().isCastle() )
	{
	}
	else {
		bool bIsWhite = root->GetBestMove()->GetMove().getPiece()->isWhite();
		ChessPiece* tempPiece = bIsWhite ? 
		WhitePlayer::GetInstance()->FindPiece(root->GetBestMove()->GetMove().getPiece() ) :
		BlackPlayer::GetInstance()->FindPiece(root->GetBestMove()->GetMove().getPiece() );
		
		root->GetBestMove()->GetMove().setPiece(tempPiece);
	}
	
	ChessPiece* CapturedPiece = root->GetBestMove()->GetMove().getCapturedPiece();
	if (CapturedPiece)
	{
		bool bIsWhite2 = CapturedPiece->isWhite();
		ChessPiece* tempPiece2 = bIsWhite2? WhitePlayer::GetInstance()->FindPiece(CapturedPiece) :
		BlackPlayer::GetInstance()->FindPiece(CapturedPiece);
		
		root->GetBestMove()->GetMove().setCapturedPiece(tempPiece2);
	}
	
	//root->GetBestMove()->PrintNode();
	root->GetBestMove()->MakeMove();
	//WhitePlayer::GetInstance()->PrintPlayer();
	fflush(stdout);
	ClearTree();
	UpdateDepth(root, 0);
	
	/*for (int i = 0; i <= FixedPly; i++)
	{
		TreeLevels.erase(TreeLevels.begin() );
	}
	
	for (int i = 0; i <= FixedPly; i++)
	{
		TreeLevels.push_back(new TreeLevel(i&1 == 1));
	}*/
	//ChessLog::GetInstance()->printLog();
	//ChessBoard::GetInstance()->PrintBoard();
	//TreeLevels.push_back(new TreeLevel(false));
	//TreeLevels.push_back(new TreeLevel(true));
	//TreeLevels.push_back(new TreeLevel(false));
	
	/*int lim = root->GetNumChildren();
	
	for (int i = lim-1; i >= 0 ; i--)
	{
		MoveNode* currNode = root->GetChild(i);
		//if( !(currNode == TreeLevels[1]->GetBestMove() ) )
		{
			currNode->Erase();
			delete(currNode);
			root->removeChild(i);
		}
		//else {
		{
			continue;
		}
	}
	
	int k;
	k = 0;*/
	/*for (int it = 0; it < Ply; it++)
	{
		if (it != 0)
			currParent->MakeMove();
		
		for (int i = 0; i < currPlayer->GetNumPiecesLeft(); i++)
		{
			currPiece = currPlayer->GetPiece(i);
			currLocation = currPiece->GetLocation();
			currPiece->GetLegalMoves(LegalMoves);
			for (int i = 0; i < LegalMoves.size(); i++)
			{
				ChessPiece* bCapturePiece = ChessBoard::GetInstance()->GetPiece(LegalMoves[i]->row, LegalMoves[i]->col);
				if (bCapturePiece)
				{
					MoveNode* currNode = new MoveNode(new ChessMove(currPiece, currLocation.row, currLocation.col, LegalMoves[i]->row, LegalMoves[i]->col, true), root, true);
					AddNode(currNode, currParent);
				}
				else
				{
					MoveNode* currNode = new MoveNode(new ChessMove(currPiece, currLocation.row, currLocation.col, LegalMoves[i]->row, LegalMoves[i]->col, false), root, true);
					AddNode(currNode, currParent);
				}
			}
			LegalMoves.clear();
		}
		
		if (it != 0)
			currParent->UndoMove();
	}*/
}

void MoveTree::UpdateDepth(MoveNode* Parent, int depth)
{
	for (int i = 0; i < Parent->GetNumChildren(); i++)
	{
		UpdateDepth(Parent->GetChild(i), depth+1);
	}
	Parent->depth = depth;
}

void MoveTree::ClearTree()
{
	//Preserve the branch of the move we selected, delete everything else
	MoveNode* tempRoot = root->GetBestMove()->Copy();
	root->Erase();
	delete(root);
	root = tempRoot;
}

MoveNode * MoveTree::MoveToMoveNode(const ChessMove& currMove, MoveNode* Parent, bool bIsMinLevel, ChessBoard* _ChessBoardInstance)
{
	MoveNode* currNode = NULL;
	currNode = new MoveNode(currMove, Parent, false, !bIsMinLevel, attachedChromosome, _ChessBoardInstance);
	return currNode;
}

void MoveTree::FillNode(int Ply, MoveNode* Parent, bool bIsWhite, WhitePlayer* _WhitePlayer, BlackPlayer* _BlackPlayer, ChessBoard* _ChessBoardInstance)
{
	if (Parent->depth == Ply)
	{
		Parent->CalculateScore();
		return;
	}
	else {
		if (Parent->depth != 0)
			Parent->MakeMove();
		
		std::vector<ChessMove> LegalMoves;
		
		Player* currPlayer;
		ChessPiece* currPiece;
		Move currLocation;
		if (bIsWhite)
			currPlayer = _WhitePlayer;
		else
			currPlayer = _BlackPlayer;
		//currPlayer = bIsWhite ? WhitePlayer::GetInstance() : BlackPlayer::GetInstance();
		for (int it = 0; it < currPlayer->GetNumPiecesLeft(); it++)
		{
			currPiece = currPlayer->GetPiece(it);
			currLocation = currPiece->GetLocation();
			currPiece->GetLegalMoves(LegalMoves);
			for (int i = 0; i < LegalMoves.size(); i++)
			{
				
				MoveNode * currNode = MoveToMoveNode(LegalMoves[i], Parent, Parent->IsMinLevel(), _ChessBoardInstance);
				
				currNode->depth = Parent->depth+1;
				AddNode(currNode, Parent);
				FillNode(Ply, currNode, !bIsWhite, _WhitePlayer, _BlackPlayer, _ChessBoardInstance);
			}
			LegalMoves.clear();
		}
		
		if (Parent->depth != 0)
			Parent->UndoMove();
		
	}

}

void MoveTree::FillAlphaBetaNode(int Ply, MoveNode* Parent, bool bIsWhite, WhitePlayer* _WhitePlayer, BlackPlayer* _BlackPlayer, ChessBoard* _ChessBoardInstance)
{
	
	if (Parent->depth == Ply)
	{
		Parent->CalculateScore();
		return;
	}
	
	if (Parent->depth != 0) // for root
		Parent->MakeMove();
	else {
		assert(bIsWhite != Parent->IsMaxLevel() );
	}

	
	Player* currPlayer;
	ChessPiece* currPiece;
	Move currLocation;
	if (bIsWhite)
		currPlayer = _WhitePlayer;
	else
		currPlayer = _BlackPlayer;
	//currPlayer = bIsWhite ? WhitePlayer::GetInstance() : BlackPlayer::GetInstance();
	
	bool bBreakingOut = false;
	
	//TreeLevel* currLevel = GetLevelFromDepth(Parent->depth+1);
	//TreeLevel* parentLevel = GetLevelFromDepth(Parent->depth);
	
	MoveNode* currBestMove = NULL;
	
	int localMax = -214748324;
	//Beta is for maximum branches
	int localMin = 0x07FFFFFF;
	//Alpha is for minimum branches
	
	/*if (Parent->GetNumChildren()  != 0 )
	{
		MoveNode* currNode = NULL;
		//When we 'saved' a branch after last evaluation
		for (int i = 0; i < Parent->GetNumChildren(); i++)
		{
			currNode = Parent->GetChild(i);
			FillAlphaBetaNode( Ply, currNode, !bIsWhite );
			int currScore = currNode->GetBoardScore();
		
			if ( currLevel->isMin() )
			{
				//If leaf level is min, parent level must be max
				if (currScore < parentLevel->GetMax() && (Parent->depth > 0))
				{
					//Pop Node
					bBreakingOut = true;
					localMin = currScore;
#ifndef SUPPRESS_OUTPUT
					printf("Beta Pruned!");
#endif
					break;
				}
				else 
				{
					//This must be a beta branch
					if (currScore < localMin)
					{
						localMin = currScore;
						currBestMove = currNode;
					}
				}
			
			}
			else 
			{
				//If leaf level is max, parent level must be min
				
				if (currScore > parentLevel->GetMin()  && (Parent->depth > 0) )
				{
					//Pop Node
					bBreakingOut = true;
					localMax = currScore;
#ifndef SUPPRESS_OUTPUT
					printf("beta pruned!");
#endif
					break;
				}
				else 
				{
					//This must be an alpha branch
					if (currScore > localMax)
					{
						localMax = currScore;
						currBestMove = currNode;
					}
				}
			
			}
		}
	}
	else*/
	//{
		std::vector<MoveNode*> MoveNodes;
		std::vector<ChessMove> LegalMoves;
		
		for (int it = 0; it < currPlayer->GetNumPiecesLeft(); it++)
		{
			currPiece = currPlayer->GetPiece(it);
			currLocation = currPiece->GetLocation();
			currPiece->GetLegalMoves(LegalMoves);	
		
			//In maximum branches, we take the worst case scenario, ie. the lowest possible score
			//In minimum branches, we take the best case scenario ie. the highest possible score
		
			for (int i = 0; i < LegalMoves.size(); i++)
			{
				MoveNode * currNode = MoveToMoveNode(LegalMoves[i], Parent, Parent->IsMinLevel(), _ChessBoardInstance);
				currNode->depth = Parent->depth+1;
				MoveNodes.push_back(currNode);
			}
			LegalMoves.clear();
		}
		/*ChessBoard* Copy;
		if (Parent->depth == 0)
		{
			Copy = ChessBoard::GetInstance()->Copy();
		}*/
		for (int i = MoveNodes.size()-1; i >= 0; i--)
		{
			MoveNode * currNode = MoveNodes[i];
			currNode->depth = Parent->depth+1;
			AddNode(currNode, Parent);
			MoveNodes.pop_back();
#ifdef USE_THREADS
				if (Parent->depth == 0)
				{
					pthread_t Thread1;
					int iret1;
					struct ThreadStruct pThreadInfo;
					pThreadInfo.bIsWhite = bIsWhite;
					pThreadInfo.Ply = Ply;
					pThreadInfo.CurrentNode = currNode;
					pThreadInfo.Tree = this;
					//pThreadInfo.ChessBoardInstance = Copy;
					if (m_iCurrentRunningThreads == NUM_THREADS)
					{
						for (int th = 0; th < NUM_THREADS; th++)
						{
							pthread_join(TreeThreads[th], NULL);
							m_iCurrentRunningThreads--;
						}
						//m_iCurrentRunningThreads = 0;
					}
					//ThreadStartFunction(&pThreadInfo);
					//iret1 = pthread_create( &(TreeThreads[m_iCurrentRunningThreads]), NULL, ThreadStartFunction, &pThreadInfo);
					iret1 = pthread_create(&Thread1, NULL, ThreadStartFunction, &pThreadInfo);
					TreeThreads[m_iCurrentRunningThreads] = Thread1;
					m_iCurrentRunningThreads++;
					
					//if (it == (currPlayer->GetNumPiecesLeft() - 1) )
					{
						if (i == 0 )
						{
							for (int th = 0; th < NUM_THREADS; th++)
							{
								pthread_join(TreeThreads[th], NULL);
							}
						}
					}
					//printf("Created thread successfully");
					//pthread_join(TreeThreads[m_iCurrentRunningThreads-1], NULL);
					//TreeThreads.pop_back();
					//m_iCurrentRunningThreads--;
					//pthread_join(TreeThreads[m_iCurrentRunningThreads], NULL);
					
					
					//pthread_join( thread1, NULL);
					//printf("Thread 1 has returned");
				}
				else 
				{
					//_ChessBoardInstance->PrintBoard();
					//_ChessBoardInstance->GetLog()->printLog();
					FillAlphaBetaNode( Ply, currNode, !bIsWhite , _WhitePlayer, _BlackPlayer, _ChessBoardInstance);
				}

				//Need a copy of chess player
				//Need a copy of White player,
				//Need a copy of Black player
				//Need a copy of chess log
#else
					FillAlphaBetaNode( Ply, currNode, !bIsWhite , WhitePlayer::GetInstance(), BlackPlayer::GetInstance(), _ChessBoardInstance);
#endif
				if (Parent->depth != 0)
				{
					int currScore = currNode->GetBoardScore();
				
				/*if (Parent->depth == 0)
				{
					if (currNode->GetNumChildren() == 0)
					{
						if (currNode->UnThreadedCopy)
							Parent->SetBestMove(currNode->UnThreadedCopy);
						else
							Parent->SetBestMove(currNode);
						currBestMove = currNode;
						bBreakingOut = true;
						
						localMax = -1000000000;
						localMin = 0x07FFFFFF;
						if (Parent->IsMaxLevel() )
						{
							Parent->SetMinScore(localMax);
							Parent->SetBoardScore(localMax);
							if (Parent->UnThreadedCopy)
								Parent->UnThreadedCopy->SetBoardScore(localMax);
						}
						else 
						{
							Parent->SetMaxScore(localMin);
							Parent->SetBoardScore(localMin);
							if (Parent->UnThreadedCopy)
								Parent->UnThreadedCopy->SetBoardScore(localMin);
						}

					}
				}*/
				
					if (currScore < -1000000000 || currScore > 0x7FFFFFFF)
						ChessAssert();
					
					if (Parent->depth == 0 && !bBreakingOut)
					{
						//Check for checkmate
						//If found, stop searching
						if (currScore == -100000000 )
						{
							bBreakingOut = true;
							currBestMove = currNode;
							assert(currNode->GetMove().getStartingRow() < 8);
							//currNode->PrintNode();
						}
						else if (currScore == 0x07FFFFFF)
						{
							bBreakingOut = true;
							currBestMove = currNode;
							assert(currNode->GetMove().getStartingRow() < 8);
							//currNode->PrintNode();
						}
					}
				
					if ( currNode->IsMinLevel() && !bBreakingOut)
					{
						//If leaf level is min, parent level must be max
						if (Parent->depth > 0
							&& currScore < Parent->getParent()->GetMaxScore() 
							)
						{
							//Pop Node
							bBreakingOut = true;
							localMin = currScore;
							currBestMove = currNode;
							//printf("CurrScore: %i, NextLevelMax: %i, Pardepth:%i\n", currScore, parentLevel->GetMax(), Parent->depth );
#ifndef SUPPRESS_OUTPUT
							printf("Beta Pruned!");
#endif
							break;
						}
						else 
						{
							//This must be a beta branch
							if (currScore < localMin)
							{
								localMin = currScore;
								currBestMove = currNode;
								if (!(currNode->GetMove().isCastle() ))
									assert(currNode->GetMove().getStartingRow() < 8);
							}
						}
			
					}
					else if (!bBreakingOut)
					{
						//If leaf level is max, parent level must be min
						if (Parent->depth > 0 
							&& currScore > Parent->getParent()->GetMinScore() 
							)
						{
							//Pop Node
							bBreakingOut = true;
							localMax = currScore;
							currBestMove = currNode;
							//printf("CurrScore: %i, NextLevelMin: %i, Pardepth:%i\n", currScore, parentLevel->GetMin(), Parent->depth );
#ifndef SUPPRESS_OUTPUT
							printf("beta pruned!");
#endif
							break;
						}
						else 
						{
							//This must be an alpha branch
							if (currScore > localMax)
							{
								localMax = currScore;
								currBestMove = currNode;
								if (!(currNode->GetMove().isCastle() ))
									assert(currNode->GetMove().getStartingRow() < 8);
							}
						}
					
					}
				}
			
			}
		//}
	
	for (int i = 0; i < MoveNodes.size(); i++)
	{
		MoveNodes[i]->Erase();
		delete(MoveNodes[i]);
	}
	MoveNodes.clear();
	
	if( (Parent->depth == 0) && (Parent->GetNumChildren() == 0 ) )
	{
		if(!(currPlayer->CheckForCheckMate() ))
			ChessAssert();
		else {
			//currPlayer->SetIsCheckmated(true);
			int k;
			k = 0;
		}

	}
	bool bCheckmate = false;
	if (bIsWhite && 
		_WhitePlayer->IsInCheck() && 
		(PLY > Parent->depth > 0) &&
		(Parent->GetNumChildren() == 0)  
		)
	{
			//Checkmate??
			bCheckmate = true;
			//printf("Checkmate below:\n");
			//fflush(stdout);
			//ChessBoard::GetInstance()->PrintBoard();
			currBestMove = Parent;
			if (Parent->UnThreadedCopy)
				Parent->getParent()->SetBestMove(Parent->UnThreadedCopy);
			else
				Parent->getParent()->SetBestMove(Parent);
			//std::vector<Move*> LegalMoves;
			/*for (int it = 0; it < currPlayer->GetNumPiecesLeft(); it++)
			 {
			 currPiece = currPlayer->GetPiece(it);
			 currLocation = currPiece->GetLocation();
			 currPiece->GetLegalMoves(LegalMoves);	
			 }*/
			//Parent->PrintNode();
			Player* opponentPlayer;
			if (!bIsWhite)
				opponentPlayer = _BlackPlayer;
			else
				opponentPlayer = _WhitePlayer;
			if(!(opponentPlayer->CheckForCheckMate()))
				ChessAssert();
			localMax = -100000000;
			localMin = 0x07FFFFFF;
			if (Parent->getParent()->IsMaxLevel() )
			{
				Parent->getParent()->SetMinScore(localMax);
				Parent->SetBoardScore(localMax);
				if (Parent->UnThreadedCopy)
					Parent->UnThreadedCopy->SetBoardScore(localMax);
			}
			else 
			{
				Parent->getParent()->SetMaxScore(localMin);
				Parent->SetBoardScore(localMin);
				if (Parent->UnThreadedCopy)
					Parent->UnThreadedCopy->SetBoardScore(localMin);
			}
	}
	else if( (!bIsWhite) && 
			_BlackPlayer->IsInCheck() 
			&& PLY > Parent->depth > 0
			&&(Parent->GetNumChildren() == 0)  
			)
	{
			//Checkmate??
			bCheckmate = true;
			//printf("Checkmate below:\n");
			//fflush(stdout);
			//ChessBoard::GetInstance()->PrintBoard();
			currBestMove = Parent;
			if (Parent->UnThreadedCopy)
				Parent->getParent()->SetBestMove(Parent->UnThreadedCopy);
			else
				Parent->getParent()->SetBestMove(Parent);
			//std::vector<Move*> LegalMoves;
			/*for (int it = 0; it < currPlayer->GetNumPiecesLeft(); it++)
			 {
			 currPiece = currPlayer->GetPiece(it);
			 currLocation = currPiece->GetLocation();
			 currPiece->GetLegalMoves(LegalMoves);	
			 }*/
			//Parent->PrintNode();
			Player* opponentPlayer;
			if (!bIsWhite)
				opponentPlayer = _BlackPlayer;
			else
				opponentPlayer = _WhitePlayer;
			if(!(opponentPlayer->CheckForCheckMate()))
				ChessAssert();
			localMax = -100000000;
			localMin = 0x07FFFFFF;
			if (Parent->getParent()->IsMaxLevel() )
			{
				Parent->getParent()->SetMinScore(localMax);
				Parent->SetBoardScore(localMax);
				if (Parent->UnThreadedCopy)
					Parent->UnThreadedCopy->SetBoardScore(localMax);
			}
			else 
			{
				Parent->getParent()->SetMaxScore(localMin);
				Parent->SetBoardScore(localMin);
				if (Parent->UnThreadedCopy)
					Parent->UnThreadedCopy->SetBoardScore(localMin);
			}
	}
	else if (Parent->GetNumChildren() == 0)
	{
		//Stalemated
		Parent->SetBoardScore(0);
	}
	//Made it all the way through the children
	else if (Parent->depth > 0)
	{
		if (Parent->getParent()->IsMaxLevel() )
		{
			if ( localMax < Parent->getParent()->GetMinScore() )
			{
				Parent->getParent()->SetMinScore(localMax);
				//assert(currBestMove);
				if( currBestMove == NULL )
				{
					PrintTree();
				}
				if (currBestMove->UnThreadedCopy)
					Parent->getParent()->SetBestMove(currBestMove->UnThreadedCopy);
				else
					Parent->getParent()->SetBestMove(currBestMove);
				//currBestMove->getParent()->PrintNode();
			}
		}
		else 
		{
			if (localMin > Parent->getParent()->GetMaxScore() )
			{
				Parent->getParent()->SetMaxScore(localMin);
				//assert(currBestMove);
				if( currBestMove == NULL )
				{
					PrintTree();
				}
				if (currBestMove->UnThreadedCopy)
					Parent->getParent()->SetBestMove(currBestMove->UnThreadedCopy);
				else
					Parent->getParent()->SetBestMove(currBestMove);
				//currBestMove->getParent()->PrintNode();
			}
		}
	}
	
	if (Parent->depth != 0)
	{
		if (! (Parent->IsLeafNode() ) && Parent->GetNumChildren() != 0)
		{
			if (!bCheckmate)
			{
				Parent->SetBoardScore(Parent->IsMinLevel()? localMax : localMin);
				if (Parent->UnThreadedCopy)
					Parent->UnThreadedCopy->SetBoardScore(Parent->IsMinLevel()? localMax : localMin);
				assert(currBestMove);
				Parent->SetBestMove(currBestMove);
			}
		}
	}
	if (Parent->depth != 0)
		Parent->UndoMove();
	
}

int MoveTree::FindScore(MoveNode* Parent, bool bMax)
{
	if ( Parent->IsLeafNode() )
		return Parent->GetBoardScore();
	if (bMax)
	{
		int Maximum = -1;
		for (int i = 0; i < Parent->GetNumChildren(); i++)
		{
			int Max = FindScore(Parent->GetChild(i), !bMax );
			if ( Max > Maximum )
			{
				if (Parent->depth == 0)
					BestMove = Parent->GetChild(i);
				Maximum = Max;
			}
			
		}
		if ( !(Parent->IsLeafNode() ) )
			Parent->SetBoardScore(Maximum);
		
		return Maximum;
	}
	else {
		int Minimum = 0x7FFFFFFF;
		for (int i = 0; i < Parent->GetNumChildren(); i++)
		{
			int Min = FindScore(Parent->GetChild(i), !bMax );
			if ( Min < Minimum )
			{
				if (Parent->depth == 0)
					BestMove = Parent->GetChild(i);
				Minimum = Min;
			}
		}
		if ( !(Parent->IsLeafNode() ) )
			Parent->SetBoardScore(Minimum);
		return Minimum;
	}

}

TreeLevel * MoveTree::GetLevelFromDepth(int depth)
{
	return TreeLevels[depth];
}

void MoveTree::AddNode(MoveNode* move, MoveNode* parent)
{
	parent->AddChild(move);
}

void MoveTree::PrintTree()
{
	for (int i = 0; i < root->GetNumChildren(); i++)
	{
		root->GetChild(i)->PrintNode();
	}
}

void MoveTree::PruneBranches()
{
	
	/*if (! (root->IsNullMove() ) )
	{
		bool bFoundMove = false;
		ChessMove* lastMove = ChessLog::GetInstance()->getLastMove();
		{
			int lim = root->GetNumChildren();
			for (int i = (lim - 1); i >= 0; i-- )
			{
				ChessMove* childMove = root->GetChild(i)->GetMove();
				if( !(childMove->isEqual(lastMove)) )
				{
					root->GetChild(i)->Erase();
					delete(root->GetChild(i) );
					root->removeChild(i);
				}
				else
				{
					int k;
					k = 0;
					bFoundMove = true;
				}
			}
			if (bFoundMove)
			{
				MoveNode* newRoot = root->GetChild(0);
				assert(newRoot);
				root->Erase();
				delete(root);
				root = NULL;
				root = newRoot;
			}
			else {
				ChessBoard::GetInstance()->PrintBoard();
				ChessLog::GetInstance()->printLog();
				root = new MoveNode(new ChessMove(), NULL, false, bIsWhite, attachedChromosome);
			}

			//UpdateDepth(root, 0);
		}
		if (bFoundMove)
		{
			int lim = root->GetNumChildren();
			for (int i = (lim-1); i >= 0; i--)
			{
				root->GetChild(i)->Erase();
				delete(root->GetChild(i) );
				root->removeChild(i);
			}
			//if (bIsWhite)
			//	root->SwitchMinLevel();
		}
		root->depth = 0;
	}
	else*/ {
		/*root->Erase();
		delete(root);
		root = NULL;
		root = new MoveNode(ChessMove(ChessBoard::GetInstance()), NULL, false, bIsWhite, attachedChromosome, ChessBoard::GetInstance());
		root->depth = 0;*/
	}
}

