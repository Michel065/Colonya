#pragma once
#include <vector>
#include <set>
#include <utility>

struct Point {
    float x, y;
};

class CalculateurDeRecouvrement {
public:
    std::vector<std::pair<int, int>> get_chunk(const std::vector<Point>& points);

private:
    bool point_in_polygon(Point p, const std::vector<Point>& poly);
    bool segments_intersect(Point a1, Point a2, Point b1, Point b2);
    bool polygon_intersects_cell(const std::vector<Point>& poly, int i, int j);
    Point make_point(int x, int y);
};
