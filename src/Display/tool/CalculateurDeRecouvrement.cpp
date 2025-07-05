#include "CalculateurDeRecouvrement.h"
#include <cmath>
#include <algorithm>

using Cell = std::pair<int, int>;

bool CalculateurDeRecouvrement::point_in_polygon(Point p, const std::vector<Point>& poly) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        float xi = poly[i].x, yi = poly[i].y;
        float xj = poly[j].x, yj = poly[j].y;
        if (((yi > p.y) != (yj > p.y)) &&
            (p.x < (xj - xi) * (p.y - yi) / (yj - yi + 1e-9f) + xi))
            inside = !inside;
    }
    return inside;
}

bool CalculateurDeRecouvrement::segments_intersect(Point a1, Point a2, Point b1, Point b2) {
    auto cross = [](Point a, Point b) { return a.x * b.y - a.y * b.x; };
    auto sub = [](Point a, Point b) { return Point{a.x - b.x, a.y - b.y}; };

    Point da = sub(a2, a1);
    Point db = sub(b2, b1);
    Point dp = sub(a1, b1);

    float denom = cross(da, db);
    if (std::fabs(denom) < 1e-9f) return false;

    float ua = cross(db, dp) / denom;
    float ub = cross(da, dp) / denom;
    return ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1;
}

Point CalculateurDeRecouvrement::make_point(int x, int y) {
    return Point{static_cast<float>(x), static_cast<float>(y)};
}

bool CalculateurDeRecouvrement::polygon_intersects_cell(const std::vector<Point>& poly, int i, int j) {
    Point corners[4] = {
        make_point(i, j),
        make_point(i + 1, j),
        make_point(i + 1, j + 1),
        make_point(i, j + 1)
    };

    for (const auto& corner : corners)
        if (point_in_polygon(corner, poly)) return true;

    for (const auto& pt : poly)
        if (pt.x >= i && pt.x <= i+1 && pt.y >= j && pt.y <= j+1)
            return true;

    for (size_t k = 0; k < poly.size(); ++k) {
        Point a = poly[k];
        Point b = poly[(k+1)%poly.size()];
        for (int l = 0; l < 4; ++l) {
            Point c = corners[l];
            Point d = corners[(l+1)%4];
            if (segments_intersect(a, b, c, d)) return true;
        }
    }

    return false;
}

std::vector<Cell> CalculateurDeRecouvrement::get_chunk(const std::vector<Point>& input) {
    std::set<Cell> result;

    if (input.empty()) return {};

    if (input.size() == 1) {
        int x = std::floor(input[0].x);
        int y = std::floor(input[0].y);
        result.insert({x, y});
    }

    else if (input.size() == 2) {
        Point a = input[0], b = input[1];
        int x0 = std::floor(a.x), y0 = std::floor(a.y);
        int x1 = std::floor(b.x), y1 = std::floor(b.y);

        int dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            result.insert({x0, y0});
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    }

    else {
        float xmin = input[0].x, xmax = input[0].x;
        float ymin = input[0].y, ymax = input[0].y;
        for (const auto& p : input) {
            xmin = std::min(xmin, p.x);
            xmax = std::max(xmax, p.x);
            ymin = std::min(ymin, p.y);
            ymax = std::max(ymax, p.y);
        }

        for (int i = std::floor(xmin); i <= std::ceil(xmax); ++i) {
            for (int j = std::floor(ymin); j <= std::ceil(ymax); ++j) {
                Point center = {i + 0.5f, j + 0.5f};
                if (point_in_polygon(center, input))
                    result.insert({i, j});
                else if (polygon_intersects_cell(input, i, j))
                    result.insert({i, j});
            }
        }
    }

    return std::vector<Cell>(result.begin(), result.end());
}
