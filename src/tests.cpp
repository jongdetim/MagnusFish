#include "Board.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Include your chess engine header
// #include "your_chess_engine.h"

struct TestCase
{
    std::string fen;
    bool expectedResult;
};

void runTests(const std::vector<TestCase>& tests)
{
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
		board.getPieceIndexes(board.kings & board.pieces[board.sideToMove]);
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

int main() {
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
    
    runTests(tests);
    
    return 0;
}