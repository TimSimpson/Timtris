#include "TimtrisScore.hpp"

namespace lp3 { namespace timtris {

namespace {
	const lp3::gfx::BoxTiles blue_box_tiles = {
		96 + 9,
		96 + 5,
		96 + 4,
		96,
		96,
		96 + 2,
		96 + 5,
		96 + 7
	};
}
TimtrisScore::TimtrisScore()
:   layer{{8, 8}},
    score{0},
    lines{0}
{
    //16 x 5
    // 14 x 3
    int width = 8;// of score box
    int height = 8;

	
    //96
	layer.fill({ 0, 0 }, { width, height }, 0);
	layer.write_box(blue_box_tiles, { 0, 0 }, { width, height });
        
	layer.write({ 1, 1 }, "Score ");
	layer.write({ 1, 2 }, "000000");
	layer.write({ 1, 3 }, "Level ");
	layer.write({ 1, 4 }, "  Easy");
	layer.write({ 1, 5 }, "Lines ");
	layer.write({ 1, 6 }, "     0");
}

void TimtrisScore::AddLinePoints()
{
    AddPoints(100);
    lines ++;
	const std::string line_text = str(boost::format("%d") % lines);
	layer.write({ 1, 6 }, line_text.c_str());
}

void TimtrisScore::AddPoints(int points)
{
    //char buf[14];
    int rem;
    int cursor; // Goes from right to left
    int tempScore;
    score += points;
    tempScore = score;
    for(cursor = 5; tempScore > 0 && cursor >= 0; cursor --)
    {
        rem = tempScore % 10;
		layer.write({ 1 + cursor, 2 }, rem + 16);//rem + 16 + 32);
        tempScore /= 10;
    }
    for ( ; cursor >= 0; cursor --)
    {
		layer.write({ 1 + cursor, 2 }, std::uint8_t(0));//buf[cursor] = 32;
    }
//    layer->Write(1, 3, buf, 13);
}

gfx::TileMap & TimtrisScore::get_layer() {
	return layer;
}

int TimtrisScore::GetScore()
{
    return score;
}

}   }
