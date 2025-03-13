#include "Board.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

struct TestCase
{
    std::string fen;
    bool		expectedResult;
};

struct MoveCountTest
{
    std::string fen;
    size_t		expectedMoves;
};

void	testMoveCounts()
{
	std::vector<MoveCountTest> tests =
	{
        // {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 20},
        {"r7/1P4p1/8/8/8/8/P6p/8 w KQkq - 0 1", 10},
        {"8/1P4p1/8/8/8/8/P6p/6R1 b KQkq - 0 1", 10},
        // {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 48},
        // {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 14},
        // {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6},
        // {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 44},
        // {"rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3", 31},
        // {"k7/8/K7/8/8/8/8/8 w - - 0 1", 3},
        // {"r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1", 27},
        // {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 26},
        // {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 24}
    };

	Board	board;
    int passed = 0;
    int total = tests.size();
    
    std::cout << "=== Running Legal Move Count Tests ===\n";
    
    for (size_t i = 0; i < tests.size(); ++i)
	{
        const auto& test = tests[i];
		board.reset();
		board.parseFen(tests[i].fen.c_str());
		std::cout << board << std::endl;
		board.generateMoves();
        
        std::cout << "FEN: " << test.fen << "\n";

        bool passed_test = (board.moveList.size() == test.expectedMoves);
        
		std::cout << board << std::endl;
        std::cout << "Expected: " << test.expectedMoves << "\n";
        std::cout << "Generated: " << board.moveList.size() << "\n";
        std::cout << "Result: " << (passed_test ? "PASSED" : "FAILED") << "\n\n";
        
        if (passed_test)
		{
            passed++;
        }
    }
    
    std::cout << "=== Move Count Test Summary ===\n";
    std::cout << "Passed: " << passed << "/" << total << " tests\n";
    std::cout << "Success rate: " << (total > 0 ? (passed * 100.0 / total) : 0) << "%\n";
}

void	testChecks()
{
	std::vector<TestCase> tests =
	{
		{"rnbqkbnr/pppp1ppp/5N2/4p3/4P3/8/PPPP1PPP/RNBQKB1R b KQkq - 0 1", true},
		{"r1bqkrn1/pppp1ppp/2n3b1/4p2Q/B3P3/5N2/PPPPRPPP/RN2KB2 w Qq - 0 1", false},
		{"r1bqkrn1/pppp1ppp/2n3b1/4p2Q/B3P3/5N2/PPPPRPPP/RN2KB2 b Qq - 0 1", false},
		{"r1bqkrn1/pppp1ppp/2n3b1/7Q/B7/5N2/PPPPRPPP/RN2KB2 b Qq - 0 1", true},
		{"r1bqkrnR/pppp1ppp/2n3b1/8/B7/4PN2/PPPPQPPP/RN2KB2 b Qq - 0 1", false},
		{"r1bqkrn1/pppP1ppp/2n3b1/8/B7/4PN2/PPP1QPPP/RN2KB1R b Qq - 0 1", true},
		{"r1bq1rn1/pppP1ppp/2n1k1b1/8/B7/4PN2/PPP1QPPP/RN2KB1R w Q - 0 1", false},
		{"r1bq1rn1/pppP2pp/2n1k1b1/8/B7/4PN2/PPP1QpPP/RN2KB1R w Q - 0 1", true},
		{"r1bq1rn1/pppP2pp/2n1k1b1/8/B7/4PNK1/PPP1QpPP/RN3B1R b - - 0 1", false},
		{"r1b2b2/pppPr1pp/2n1k3/8/B7/K3PN1q/1PP1QpnP/RN3B1R w - - 0 1", false},
		{"rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", false},
		{"rnbq2nr/ppp1bpp1/4p3/1B1kP1Np/1PPp4/5P2/P2P2PP/RNBQK2R b KQ c3 0 1", true},
    };

	Board	board;
    int passed = 0;
    int total = tests.size();
    
    std::cout << "=== Running inCheck() Tests ===\n";
    
    for (size_t i = 0; i < tests.size(); ++i)
	{
        const auto& test = tests[i];
        std::cout << "Test #" << (i + 1) << std::endl;
        std::cout << "FEN: " << test.fen << std::endl;
        std::cout << "Expected: " << (test.expectedResult ? "true" : "false") << "\n" << std::endl;
		board.reset();
		board.parseFen(test.fen.c_str());

		std::cout << board << std::endl;
		board.getPieceIndexes(board.pieces[KING] & board.pieces[board.sideToMove]);
        bool result = board.inCheck(board.pieceSQs[0]);
        
        bool passed_test = (result == test.expectedResult);
        
		if (passed_test == true)
		{
 			std::cout << GREEN << "PASSED";
		}
		else
		{
			std::cout << RED << "FAILED";
		}
		std::cout << RESET << std::endl << std::endl;
       
        if (passed_test)
		{
            passed++;
        }
    }
    
    std::cout << "=== Test Summary ===\n";
    std::cout << "Passed: " << passed << "/" << total << " tests\n";
    std::cout << "Success rate: " << (total > 0 ? (passed * 100.0 / total) : 0) << "%\n";
}

int main()
{
    // testChecks();
	testMoveCounts();
}
