#pragma once

#include "geometry/base.hpp"
#include "graph/base.hpp"

SparseGraph<int, int> GetDelaunayGraph(const std::vector<Point2D>& position_list);