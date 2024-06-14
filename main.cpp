#include <vector>
#include <utility>
#include <algorithm>
#include <thread>
#include <chrono>
#include <atomic>
#include <cmath>
#include <random>
#include <array>
#include <chrono>
#include <memory>
#include <numeric>
#include <stack>
#include <list>

#include <iostream>

#include "SZCommon.h"

#define SHOW_DEBUG 0

void print_board(const std::vector<std::vector<int>> &board)
{
    int board_row = board.size();
    int board_column = board_row > 0 ? board[0].size() : 0;
    for (int r = 0; r < board_row; r++)
    {
        printf("{");
        for (int c = 0; c < board_column; c++)
        {
            printf("%2d, ", board[r][c]);
        }
        printf("},\n");
    }
}

/**
 * Mines中所有坐标均为(row, column)
 */
namespace Mines
{
    const int ENUM_LIMIT = 55;

    double rand_value(int64_t &seed)
    {
        seed = seed * 1103515245ll + 12345ll;
        seed = seed % 0x7fffffffll;
        return (seed * 1.0) / (0x7fffffffll * 1.0);
    }

    void shuffle_vector(std::vector<int> &v, int64_t seed)
    {
        for (int i = 0; i < static_cast<int>(v.size()); i++)
        {
            double rv = rand_value(seed);
            int j = std::floor(rv * (i + 1));
            std::swap(v[i], v[j]);
        }
    }

    bool unsolvable_structure(const std::vector<std::vector<int>> &BoardCheck)
    {
        const int row = BoardCheck.size();
        const int column = BoardCheck[0].size();
        std::vector<std::vector<int>> Board(row, std::vector<int>(column, 0));
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                if (BoardCheck[i][j] == -1)
                {
                    Board[i][j] = -1;
                }
            }
        }
        for (int i = 0; i < row - 2; i++)
        {
            // 检查左右两侧的工
            if (i < row - 3)
            {
                if ((Board[i][0] == -1 && Board[i][1] == -1 && Board[i + 3][0] == -1 && Board[i + 3][1] == -1 && Board[i + 1][0] + Board[i + 2][0] == -1) || (Board[i][column - 1] == -1 && Board[i][column - 2] == -1 && Board[i + 3][column - 1] == -1 && Board[i + 3][column - 2] == -1 && Board[i + 1][column - 1] + Board[i + 2][column - 1] == -1))
                {
                    return true;
                }
            }
            if ((Board[i][2] == -1 && Board[i + 1][2] == -1 && Board[i + 2][2] == -1 && Board[i + 1][0] + Board[i + 1][1] == -1) || (Board[i][column - 3] == -1 && Board[i + 1][column - 3] == -1 && Board[i + 2][column - 3] == -1 && Board[i + 1][column - 1] + Board[i + 1][column - 2] == -1) || (Board[i][0] == -1 && Board[i][1] == -1 && Board[i + 1][1] == -1 && Board[i + 2][1] == -1 && Board[i + 2][0] == -1 && Board[i + 1][0] == 0) || (Board[i][column - 1] == -1 && Board[i][column - 2] == -1 && Board[i + 1][column - 2] == -1 && Board[i + 2][column - 2] == -1 && Board[i + 2][column - 1] == -1 && Board[i + 1][column - 1] == 0))
            {
                return true;
            }
            if (i < row - 3)
            {
                if ((Board[i][2] == -1 && Board[i + 3][2] == -1 && Board[i + 1][0] + Board[i + 1][1] == -1 && Board[i + 1][1] + Board[i + 2][1] == -1 && Board[i + 2][1] + Board[i + 2][0] == -1) || (Board[i][column - 3] == -1 && Board[i + 3][column - 3] == -1 && Board[i + 1][column - 1] + Board[i + 1][column - 2] == -1 && Board[i + 1][column - 2] + Board[i + 2][column - 2] == -1 && Board[i + 2][column - 2] + Board[i + 2][column - 1] == -1))
                {
                    return true;
                }
            }
        }
        for (int j = 0; j < column - 2; j++)
        {
            // 检查上下两侧
            if (j < column - 3)
            {
                if ((Board[0][j] == -1 && Board[1][j] == -1 && Board[0][j + 3] == -1 && Board[1][j + 3] == -1 && Board[0][j + 1] + Board[0][j + 2] == -1) || (Board[row - 1][j] == -1 && Board[row - 2][j] == -1 && Board[row - 1][j + 3] == -1 && Board[row - 2][j + 3] == -1 && Board[row - 1][j + 1] + Board[row - 1][j + 2] == -1))
                {
                    return true;
                }
            }
            if ((Board[2][j] == -1 && Board[2][j + 1] == -1 && Board[2][j + 2] == -1 && Board[0][j + 1] + Board[1][j + 1] == -1) || (Board[row - 3][j] == -1 && Board[row - 3][j + 1] == -1 && Board[row - 3][j + 2] == -1 && Board[row - 1][j + 1] + Board[row - 2][j + 1] == -1) || (Board[0][j] == -1 && Board[1][j] == -1 && Board[1][j + 1] == -1 && Board[1][j + 2] == -1 && Board[0][j + 2] == -1 && Board[0][j + 1] == 0) || (Board[row - 1][j] == -1 && Board[row - 2][j] == -1 && Board[row - 2][j + 1] == -1 && Board[row - 2][j + 2] == -1 && Board[row - 1][j + 2] == -1 && Board[row - 1][j + 1] == 0))
            {
                return true;
            }
            if (j < column - 3)
            {
                if ((Board[2][j] == -1 && Board[2][j + 3] == -1 && Board[0][j + 1] + Board[1][j + 1] == -1 && Board[1][j + 1] + Board[1][j + 2] == -1 && Board[1][j + 2] + Board[0][j + 2] == -1) || (Board[row - 3][j] == -1 && Board[row - 3][j + 3] == -1 && Board[row - 1][j + 1] + Board[row - 2][j + 1] == -1 && Board[row - 2][j + 1] + Board[row - 2][j + 2] == -1 && Board[row - 2][j + 2] + Board[row - 1][j + 2] == -1))
                {
                    return true;
                }
            }
        }
        if ((Board[0][2] == -1 && Board[1][2] == -1 && Board[0][0] + Board[0][1] == -1) || (Board[2][0] == -1 && Board[2][1] == -1 && Board[0][0] + Board[1][0] == -1) || (Board[0][column - 3] == -1 && Board[1][column - 3] == -1 && Board[0][column - 1] + Board[0][column - 2] == -1) || (Board[2][column - 1] == -1 && Board[2][column - 2] == -1 && Board[0][column - 1] + Board[1][column - 1] == -1) || (Board[row - 1][2] == -1 && Board[row - 2][2] == -1 && Board[row - 1][0] + Board[row - 1][1] == -1) || (Board[row - 3][0] == -1 && Board[row - 3][1] == -1 && Board[row - 1][0] + Board[row - 2][0] == -1) || (Board[row - 1][column - 3] == -1 && Board[row - 2][column - 3] == -1 && Board[row - 1][column - 1] + Board[row - 1][column - 2] == -1) || (Board[row - 3][column - 1] == -1 && Board[row - 3][column - 2] == -1 && Board[row - 1][column - 1] + Board[row - 2][column - 1] == -1) || (Board[0][1] + Board[1][1] + Board[1][0] == -3 && Board[0][0] == 0) || (Board[0][column - 2] + Board[1][column - 2] + Board[1][column - 1] == -3 && Board[0][column - 1] == 0) || (Board[row - 1][column - 2] + Board[row - 2][column - 2] + Board[row - 2][column - 1] == -3 && Board[row - 1][column - 1] == 0) || (Board[row - 1][1] + Board[row - 2][1] + Board[row - 2][0] == -3 && Board[row - 1][0] == 0) || (Board[2][2] == -1 && Board[0][1] + Board[1][1] == -1 && Board[1][0] + Board[1][1] == -1) || (Board[row - 3][2] == -1 && Board[row - 1][1] + Board[row - 2][1] == -1 && Board[row - 2][0] + Board[row - 2][1] == -1) || (Board[row - 3][column - 3] == -1 && Board[row - 1][column - 2] + Board[row - 2][column - 2] == -1 && Board[row - 2][column - 1] + Board[row - 2][column - 2] == -1) || (Board[2][column - 3] == -1 && Board[0][column - 2] + Board[1][column - 2] == -1 && Board[1][column - 1] + Board[1][column - 2] == -1))
        {
            // 检查四个角
            return true;
        }
        for (int i = 0; i < row - 2; i++)
        {
            // 找中间的工、回、器形结构
            for (int j = 0; j < column - 2; j++)
            {
                if (j < column - 3)
                {
                    if (Board[i][j] == -1 && Board[i + 1][j] == -1 && Board[i + 2][j] == -1 && Board[i][j + 3] == -1 && Board[i + 1][j + 3] == -1 && Board[i + 2][j + 3] == -1 && Board[i + 1][j + 1] + Board[i + 1][j + 2] == -1)
                    {
                        return true;
                    }
                }
                if (i < row - 3)
                {
                    if (Board[i][j] == -1 && Board[i][j + 1] == -1 && Board[i][j + 2] == -1 && Board[i + 3][j] == -1 && Board[i + 3][j + 1] == -1 && Board[i + 3][j + 2] == -1 && Board[i + 1][j + 1] + Board[i + 2][j + 1] == -1)
                    {
                        return true;
                    }
                }
                if (Board[i][j] == -1 && Board[i + 1][j] == -1 && Board[i + 2][j] == -1 && Board[i][j + 1] == -1 && Board[i + 2][j + 1] == -1 && Board[i][j + 2] == -1 && Board[i + 1][j + 2] == -1 && Board[i + 2][j + 2] == -1 && Board[i + 1][j + 1] == 0)
                {
                    return true;
                }
                if (j < column - 3 && i < row - 3)
                {
                    if (Board[i][j] == -1 && Board[i + 3][j] == -1 && Board[i][j + 3] == -1 && Board[i + 3][j + 3] == -1 && Board[i + 1][j + 1] + Board[i + 2][j + 1] == -1 && Board[i + 1][j + 1] + Board[i + 1][j + 2] == -1 && Board[i + 2][j + 1] + Board[i + 2][j + 2] == -1)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void refresh_board(
        const std::vector<std::vector<int>> &board,
        std::vector<std::vector<int>> &boardofGame,
        std::vector<std::pair<int, int>> clicked_poses)
    {
        int row = board.size();
        int column = board[0].size();
        // 是否踩雷
        bool loss_flag = false;
        while (!clicked_poses.empty())
        {
            auto top = clicked_poses.back();
            clicked_poses.pop_back();
            int i = top.first;
            int j = top.second;
            if (board[i][j] > 0)
            {
                boardofGame[i][j] = board[i][j];
            }
            else if (board[i][j] == 0)
            {
                boardofGame[i][j] = 0;
                for (int m = std::max(1, i) - 1; m < std::min(row, i + 2); m++)
                {
                    for (int n = std::max(1, j) - 1; n < std::min(column, j + 2); n++)
                    {
                        if ((i != m || j != n) && (boardofGame[m][n] == 10 || boardofGame[m][n] == 12))
                        {
                            clicked_poses.push_back({m, n});
                        }
                    }
                }
            }
            else
            {
                boardofGame[i][j] = 15; // 标红雷，此处是雷，且踩到了
                loss_flag = true;
            }
        }
        // 标叉雷
        if (loss_flag)
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < column; j++)
                {
                    if (boardofGame[i][j] == 11 && board[i][j] != -1)
                    {
                        boardofGame[i][j] = 14; // 叉雷，即标错的雷
                    }
                }
            }
        }
    }

    bool isVictory(const std::vector<std::vector<int>> &board_of_game, const std::vector<std::pair<int, int>> &total_mines)
    {
        bool isVictory = true;
        for (const auto &mine : total_mines)
        {
            if (board_of_game[mine.first][mine.second] != 11)
            {
                isVictory = false;
                break;
            }
        }

        return isVictory;
    }

    std::tuple<int, int>
    refresh_matrixs(const std::vector<std::vector<int>> &board_of_game, std::vector<std::vector<std::vector<int>>> &matrix_as, std::vector<std::vector<std::pair<int, int>>> &matrix_xs, std::vector<std::vector<int>> &matrix_bs)
    {
        int row = board_of_game.size();
        int column = board_of_game[0].size();
        int unknow_block = 0;
        int is_minenum = 0;

        matrix_as.clear();
        matrix_xs.clear();
        matrix_bs.clear();

        std::vector<std::pair<int, int>> all_cell;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                if (board_of_game[i][j] >= 0 && board_of_game[i][j] < 10)
                {
                    bool isContinue = true;
                    for (int m = std::max(1, i) - 1; m < std::min(row, i + 2) && isContinue; m++)
                    {
                        for (int n = std::max(1, j) - 1; n < std::min(column, j + 2) && isContinue; n++)
                        {
                            if (board_of_game[m][n] == 10)
                            {
                                all_cell.push_back(std::make_pair(i, j));
                                isContinue = false;
                                break;
                            }
                        }
                    }
                }
                else if (board_of_game[i][j] == 10)
                {
                    bool flag = true;
                    for (int m = std::max(1, i) - 1; m < std::min(row, i + 2) && flag; m++)
                    {
                        for (int n = std::max(1, j) - 1; n < std::min(column, j + 2) && flag; n++)
                        {
                            if (board_of_game[m][n] < 10)
                            {
                                flag = false;
                                break;
                            }
                        }
                    }
                    if (flag)
                    {
                        unknow_block++;
                    }
                }
                else if (board_of_game[i][j] == 11)
                {
                    is_minenum++;
                }
            }
        }
        int p = 0;
        while (!all_cell.empty())
        {
            matrix_xs.push_back(std::vector<std::pair<int, int>>());
            matrix_bs.push_back(std::vector<int>());
            int x_0 = all_cell[0].first;
            int y_0 = all_cell[0].second;
            std::vector<std::pair<int, int>> num_cells;
            std::vector<std::pair<int, int>> temp_cells;
            int flag_num = 0;
            for (int m = std::max(1, x_0) - 1; m < std::min(row, x_0 + 2); m++)
            {
                for (int n = std::max(1, y_0) - 1; n < std::min(column, y_0 + 2); n++)
                {
                    if (board_of_game[m][n] == 10)
                    {
                        matrix_xs[p].push_back(std::make_pair(m, n));
                    }
                    if (board_of_game[m][n] == 11)
                    {
                        flag_num++;
                    }
                }
            }
            matrix_bs[p].push_back(board_of_game[x_0][y_0] - flag_num);
            num_cells.push_back(std::make_pair(x_0, y_0));
            temp_cells.push_back(std::make_pair(x_0, y_0));
            while (!temp_cells.empty())
            {
                int x_e = temp_cells[0].first;
                int y_e = temp_cells[0].second;
                temp_cells.erase(temp_cells.begin());
                for (int t = all_cell.size() - 1; t >= 1; t--)
                {
                    int x_t = all_cell[t].first;
                    int y_t = all_cell[t].second;
                    if (x_t >= x_e + 3 || x_e >= x_t + 3 || y_t >= y_e + 3 || y_e >= y_t + 3)
                    {
                        continue;
                    }
                    bool flag_be_neighbor = false;
                    for (int m = std::max(1, std::max(x_t, x_e)) - 1; m < std::min(row, std::min(x_t + 2, x_e + 2)) && !flag_be_neighbor; m++)
                    {
                        for (int n = std::max(1, std::max(y_t, y_e)) - 1; n < std::min(column, std::min(y_t + 2, y_e + 2)) && !flag_be_neighbor; n++)
                        {
                            if (board_of_game[m][n] == 10)
                            {
                                flag_be_neighbor = true;
                                break;
                            }
                        }
                    }
                    if (flag_be_neighbor)
                    {
                        int flag_num = 0;
                        for (int m = std::max(1, x_t) - 1; m < std::min(row, x_t + 2); m++)
                        {
                            for (int n = std::max(1, y_t) - 1; n < std::min(column, y_t + 2); n++)
                            {
                                if (board_of_game[m][n] == 10)
                                {
                                    if (std::find(matrix_xs[p].begin(), matrix_xs[p].end(), std::make_pair(m, n)) == matrix_xs[p].end())
                                    {
                                        matrix_xs[p].push_back(std::make_pair(m, n));
                                    }
                                }
                                if (board_of_game[m][n] == 11)
                                {
                                    flag_num++;
                                }
                            }
                        }
                        matrix_bs[p].push_back(board_of_game[x_t][y_t] - flag_num);
                        num_cells.push_back(std::make_pair(x_t, y_t));
                        temp_cells.push_back(all_cell[t]);
                        all_cell.erase(all_cell.begin() + t);
                    }
                }
            }
            matrix_as.push_back(std::vector<std::vector<int>>(num_cells.size(), std::vector<int>(matrix_xs[p].size(), 0)));
            for (int i = 0; i < static_cast<int>(num_cells.size()); i++)
            {
                for (int j = 0; j < static_cast<int>(matrix_xs[p].size()); j++)
                {
                    if (num_cells[i].first <= matrix_xs[p][j].first + 1 && matrix_xs[p][j].first <= num_cells[i].first + 1 && num_cells[i].second <= matrix_xs[p][j].second + 1 && matrix_xs[p][j].second <= num_cells[i].second + 1)
                    {
                        matrix_as[p][i][j] = 1;
                    }
                }
            }
            all_cell.erase(all_cell.begin());
            p++;
        }
        return std::make_tuple(unknow_block, is_minenum);
    }

    std::tuple<std::vector<std::vector<std::vector<int>>>, std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<int>>>
    chunk_matrix(std::vector<std::vector<int>> &matrix_a, std::vector<std::pair<int, int>> &matrix_x, std::vector<int> &matrix_b)
    {
        int block_id = 0;
        std::vector<std::vector<std::vector<int>>> matrix_as;
        std::vector<std::vector<std::pair<int, int>>> matrix_xs;
        std::vector<std::vector<int>> matrix_bs;
        while (true)
        {
            int row_num = matrix_a.size();
            int column_num = matrix_a[0].size();
            std::vector<bool> current_rows_bool(row_num, false);
            std::vector<bool> current_columns_bool(column_num, false);
            current_columns_bool[0] = true;
            std::vector<int> column_buffer = {0};
            while (true)
            {
                if (column_buffer.empty())
                {
                    break;
                }

                std::vector<int> row_buffer;
                for (auto &i : column_buffer)
                {
                    for (int idr = 0; idr < static_cast<int>(matrix_a.size()); idr++)
                    {
                        if (matrix_a[idr][i] >= 1 && !current_rows_bool[idr])
                        {
                            row_buffer.push_back(idr);
                            current_rows_bool[idr] = true;
                        }
                    }
                }
                column_buffer.clear();
                if (row_buffer.empty())
                {
                    break;
                }
                for (auto &i : row_buffer)
                {
                    for (int idc = 0; idc < static_cast<int>(matrix_a[i].size()); idc++)
                    {
                        if (matrix_a[i][idc] >= 1 && !current_columns_bool[idc])
                        {
                            column_buffer.push_back(idc);
                            current_columns_bool[idc] = true;
                        }
                    }
                }
            }
            std::vector<int> current_rows;
            std::vector<int> current_columns;
            for (int idx = 0; idx < static_cast<int>(current_columns_bool.size()); idx++)
            {
                if (current_columns_bool[idx])
                {
                    current_columns.push_back(idx);
                }
            }
            for (int idx = 0; idx < static_cast<int>(current_rows_bool.size()); idx++)
            {
                if (current_rows_bool[idx])
                {
                    current_rows.push_back(idx);
                }
            }
            std::sort(current_rows.begin(), current_rows.end(), std::greater<int>());
            current_rows.erase(std::unique(current_rows.begin(), current_rows.end()), current_rows.end());
            std::sort(current_columns.begin(), current_columns.end(), std::greater<int>());
            current_columns.erase(std::unique(current_columns.begin(), current_columns.end()), current_columns.end());

            matrix_as.push_back(std::vector<std::vector<int>>(current_rows.size(), std::vector<int>(current_columns.size(), 0)));
            matrix_bs.push_back(std::vector<int>(current_rows.size(), 0));
            matrix_xs.push_back(std::vector<std::pair<int, int>>(current_columns.size(), std::make_pair(0, 0)));
            for (int idx = 0; idx < static_cast<int>(current_rows.size()); idx++)
            {
                for (int idy = 0; idy < static_cast<int>(current_columns.size()); idy++)
                {
                    matrix_as[block_id][idx][idy] = matrix_a[current_rows[idx]][current_columns[idy]];
                }
            }
            for (int idx = 0; idx < static_cast<int>(current_rows.size()); idx++)
            {
                matrix_bs[block_id][idx] = matrix_b[current_rows[idx]];
            }
            for (int idy = 0; idy < static_cast<int>(current_columns.size()); idy++)
            {
                matrix_xs[block_id][idy] = matrix_x[current_columns[idy]];
            }

            for (auto i : current_rows)
            {
                matrix_a.erase(matrix_a.begin() + i);
                matrix_b.erase(matrix_b.begin() + i);
            }
            for (auto &j : current_columns)
            {
                for (auto &k : matrix_a)
                {
                    k.erase(k.begin() + j);
                }
                matrix_x.erase(matrix_x.begin() + j);
            }

            if (matrix_b.empty())
            {
                break;
            }

            block_id++;
        }

        return make_tuple(matrix_as, matrix_xs, matrix_bs);
    }

    void chunk_matrixes(std::vector<std::vector<std::vector<int>>> &matrix_as, std::vector<std::vector<std::pair<int, int>>> &matrix_xs, std::vector<std::vector<int>> &matrix_bs)
    {
        int block_num = matrix_bs.size();
        std::vector<std::vector<std::vector<int>>> aas;
        std::vector<std::vector<std::pair<int, int>>> xxs;
        std::vector<std::vector<int>> bbs;
        for (int i = 0; i < block_num; i++)
        {
            auto aa = matrix_as.back();
            matrix_as.pop_back();
            auto xx = matrix_xs.back();
            matrix_xs.pop_back();
            auto bb = matrix_bs.back();
            matrix_bs.pop_back();

            auto ans = chunk_matrix(aa, xx, bb);
            auto a_ = std::get<0>(ans);
            auto x_ = std::get<1>(ans);
            auto b_ = std::get<2>(ans);
            aas.insert(aas.end(), a_.begin(), a_.end());
            xxs.insert(xxs.end(), x_.begin(), x_.end());
            bbs.insert(bbs.end(), b_.begin(), b_.end());
        }
        std::swap(matrix_as, aas);
        std::swap(matrix_xs, xxs);
        std::swap(matrix_bs, bbs);
    }

    std::tuple<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>>
    solve_direct(std::vector<std::vector<std::vector<int>>> &As, std::vector<std::vector<std::pair<int, int>>> &xs, std::vector<std::vector<int>> &bs, std::vector<std::vector<int>> &board_of_game)
    {
        std::vector<std::pair<int, int>> is_mine;
        std::vector<std::pair<int, int>> not_mine;

        int block_num = bs.size();
        for (int b = block_num - 1; b >= 0; b--)
        {
            int matrix_column = xs[b].size();
            int matrix_row = bs[b].size();
            for (int i = matrix_row - 1; i >= 0; i--)
            {
                if (std::accumulate(As[b][i].begin(), As[b][i].end(), 0) == bs[b][i])
                {
                    for (int k = matrix_column - 1; k >= 0; k--)
                    {
                        if (As[b][i][k] >= 1)
                        {
                            is_mine.push_back(xs[b][k]);
                            board_of_game[xs[b][k].first][xs[b][k].second] = 11;
                            xs[b].erase(xs[b].begin() + k);
                            for (int t = 0; t < matrix_row; t++)
                            {
                                bs[b][t] -= As[b][t][k];
                                As[b][t].erase(As[b][t].begin() + k);
                            }
                            matrix_column--;
                        }
                    }
                    As[b].erase(As[b].begin() + i);
                    bs[b].erase(bs[b].begin() + i);
                    matrix_row--;
                }
            }
            for (int i = matrix_row - 1; i >= 0; i--)
            {
                if (bs[b][i] == 0)
                {
                    for (int k = matrix_column - 1; k >= 0; k--)
                    {
                        if (As[b][i][k] >= 1)
                        {
                            not_mine.push_back(xs[b][k]);
                            board_of_game[xs[b][k].first][xs[b][k].second] = 12;
                            xs[b].erase(xs[b].begin() + k);
                            for (int t = 0; t < matrix_row; t++)
                            {
                                As[b][t].erase(As[b][t].begin() + k);
                            }
                            matrix_column--;
                        }
                    }
                    As[b].erase(As[b].begin() + i);
                    bs[b].erase(bs[b].begin() + i);
                    matrix_row--;
                }
            }
            if (bs[b].size() == 0)
            {
                As.erase(As.begin() + b);
                bs.erase(bs.begin() + b);
                xs.erase(xs.begin() + b);
            }
        }

        auto ans = find_if(bs.begin(), bs.end(), [&](const std::vector<int> &b)
                           { return std::find_if(b.begin(), b.end(), [](int x)
                                                 { return x < 0; }) != b.end(); });
        if (ans != bs.end())
        {
            return std::make_tuple(std::vector<std::pair<int, int>>{}, std::vector<std::pair<int, int>>{});
        }

        chunk_matrixes(As, xs, bs);
        return make_tuple(not_mine, is_mine);
    }

    std::tuple<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>>
    solve_minus(std::vector<std::vector<std::vector<int>>> &As, std::vector<std::vector<std::pair<int, int>>> &xs, std::vector<std::vector<int>> &bs, std::vector<std::vector<int>> &board_of_game)
    {
        int block_num = bs.size();
        std::vector<int> remove_blocks_id;
        std::vector<std::pair<int, int>> not_mine;
        std::vector<std::pair<int, int>> is_mine;
        for (int b = block_num - 1; b >= 0; b--)
        {
            std::vector<int> not_mine_rel;
            std::vector<int> is_mine_rel;
            int matrix_column = xs[b].size();
            int matrix_row = bs[b].size();
            if (matrix_row <= 1)
            {
                continue;
            }
            for (int i = 1; i < matrix_row; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    std::vector<int> ADval1;
                    std::vector<int> ADvaln1;
                    bool FlagAdj = false;
                    for (int k = 0; k < matrix_column; k++)
                    {
                        if (As[b][i][k] >= 1 && As[b][j][k] >= 1)
                        {
                            FlagAdj = true;
                            continue;
                        }
                        if (As[b][i][k] - As[b][j][k] == 1)
                        {
                            ADval1.push_back(k);
                        }
                        else if (As[b][i][k] - As[b][j][k] == -1)
                        {
                            ADvaln1.push_back(k);
                        }
                    }
                    if (FlagAdj)
                    {
                        int bDval = bs[b][i] - bs[b][j];
                        if (static_cast<int>(ADval1.size()) == bDval)
                        {
                            is_mine_rel.insert(is_mine_rel.end(), ADval1.begin(), ADval1.end());
                            not_mine_rel.insert(not_mine_rel.end(), ADvaln1.begin(), ADvaln1.end());
                        }
                        else if (static_cast<int>(ADvaln1.size()) == -bDval)
                        {
                            is_mine_rel.insert(is_mine_rel.end(), ADvaln1.begin(), ADvaln1.end());
                            not_mine_rel.insert(not_mine_rel.end(), ADval1.begin(), ADval1.end());
                        }
                    }
                }
            }
            std::sort(is_mine_rel.begin(), is_mine_rel.end());
            is_mine_rel.erase(unique(is_mine_rel.begin(), is_mine_rel.end()), is_mine_rel.end());
            std::sort(not_mine_rel.begin(), not_mine_rel.end());
            not_mine_rel.erase(unique(not_mine_rel.begin(), not_mine_rel.end()), not_mine_rel.end());
            for (int i : not_mine_rel)
            {
                not_mine.push_back(xs[b][i]);
                board_of_game[xs[b][i].first][xs[b][i].second] = 12;
            }
            for (int i : is_mine_rel)
            {
                is_mine.push_back(xs[b][i]);
                board_of_game[xs[b][i].first][xs[b][i].second] = 11;
                for (auto &row : As[b])
                {
                    bs[b][&row - &As[b][0]] -= row[i];
                }
            }
            std::vector<int> del_id = not_mine_rel;
            del_id.insert(del_id.end(), is_mine_rel.begin(), is_mine_rel.end());
            std::sort(del_id.begin(), del_id.end(), std::greater<int>());
            del_id.erase(std::unique(del_id.begin(), del_id.end()), del_id.end());
            for (int i : del_id)
            {
                xs[b].erase(xs[b].begin() + i);
                for (auto &row : As[b])
                {
                    row.erase(row.begin() + i);
                }
            }
            if (xs[b].size() == 0)
            {
                remove_blocks_id.push_back(b);
            }
        }

        for (int b : remove_blocks_id)
        {
            As.erase(As.begin() + b);
            bs.erase(bs.begin() + b);
            xs.erase(xs.begin() + b);
        }

        auto notIs = solve_direct(As, xs, bs, board_of_game);
        not_mine.insert(not_mine.end(), std::get<0>(notIs).begin(), std::get<0>(notIs).end());
        is_mine.insert(is_mine.end(), std::get<1>(notIs).begin(), std::get<1>(notIs).end());
        chunk_matrixes(As, xs, bs);
        return make_tuple(not_mine, is_mine);
    }

    void get_total_mines(const std::vector<std::vector<int>> &board, std::vector<std::pair<int, int>> &total_mines)
    {
        int row = board.size();
        int column = board[0].size();

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                if (board[i][j] == -1)
                {
                    total_mines.push_back(std::make_pair(i, j));
                }
            }
        }
    }

    bool is_solvable(const std::vector<std::vector<int>> &board, int touchRow, int touchCol)
    {
        if (unsolvable_structure(board))
        {
            // 若包含不可判雷结构，则不是无猜
            return false;
        }
        int row = board.size();
        int column = board[0].size();

        std::vector<std::pair<int, int>> total_mines;
        get_total_mines(board, total_mines);

        std::vector<std::vector<int>> board_of_game(row, std::vector<int>(column, 10));
        // 10是未打开，11是标雷，12是已安全块
        // 局面大小必须超过6*6
        refresh_board(board, board_of_game, {{touchRow, touchCol}});

        if (isVictory(board_of_game, total_mines))
        {
            return true; // 暂且认为点一下就扫开也是可以的
        }
        while (true)
        {
            std::vector<std::vector<std::vector<int>>> As;
            std::vector<std::vector<std::pair<int, int>>> xs;
            std::vector<std::vector<int>> bs;

            refresh_matrixs(board_of_game, As, xs, bs);

            auto solve_direct_ans = solve_direct(As, xs, bs, board_of_game);

            std::vector<std::pair<int, int>> not_mine;
            if (std::get<0>(solve_direct_ans).empty() && std::get<1>(solve_direct_ans).empty())
            {
                auto solve_minus_ans = solve_minus(As, xs, bs, board_of_game);
                if (std::get<0>(solve_minus_ans).empty() && std::get<1>(solve_minus_ans).empty())
                {
                    return false;
                }
                else
                {
                    not_mine = std::get<0>(solve_minus_ans);
                }
            }
            else
            {
                not_mine = std::get<0>(solve_direct_ans);
            }

            refresh_board(board, board_of_game, not_mine);
            if (isVictory(board_of_game, total_mines))
            {
                return true;
            }
        }
    }

    void laymine_op(
        std::vector<std::vector<int>> &board,
        int row,
        int column,
        int minenum,
        int touchRow,
        int touchCol,
        int64_t seed = -1)
    {
        int areaOp = 9;
        if (touchRow == 0 || touchCol == 0 || touchRow == row - 1 || touchCol == column - 1)
        {
            if ((touchRow == 0 && touchCol == 0) || (touchRow == 0 && touchCol == column - 1) || (touchRow == row - 1 && touchCol == 0) || (touchRow == row - 1 && touchCol == column - 1))
            {
                areaOp = 4;
            }
            else
            {
                areaOp = 6;
            }
        }
        int area = row * column - areaOp;
        std::vector<int> board_dim(area - minenum, 0);
        std::vector<int> appendVec(minenum, -1);
        board_dim.insert(board_dim.end(), appendVec.begin(), appendVec.end());

        if (seed < 0)
        {
            std::random_device rng;
            std::shuffle(board_dim.begin(), board_dim.end(), rng);
        }
        else
        {
            shuffle_vector(board_dim, seed);
        }

        board = std::vector<std::vector<int>>(row, std::vector<int>(column, 0));
        int skip = 0;
        for (int i = 0; i < area + areaOp; i++)
        {
            int x = i % row;
            int y = i / row;
            if (x <= touchRow + 1 && touchRow <= x + 1 && y <= touchCol + 1 && touchCol <= y + 1)
            {
                skip += 1;
                continue;
            }
            if (board_dim[i - skip] < 0)
            {
                board[x][y] = -1;
                for (int j = std::max(1, (int)x) - 1; j < std::min(row, x + 2); j++)
                {
                    for (int k = std::max(1, (int)y) - 1; k < std::min(column, y + 2); k++)
                    {
                        if (board[j][k] >= 0)
                        {
                            board[j][k] += 1;
                        }
                    }
                }
            }
        }
    }

    bool laymine_solvable(
        std::vector<std::vector<int>> &board,
        int row,
        int column,
        int minenum,
        int touchRow,
        int touchCol,
        int max_times)
    {
        int times = 0;
        while (times < max_times)
        {
            laymine_op(board, row, column, minenum, touchRow, touchCol);
            times++;
            if (is_solvable(board, touchRow, touchCol))
            {
                return true;
            }
        }
        laymine_op(board, row, column, minenum, touchRow, touchCol);
        return false;
    }

    bool laymine_solvable_with_seed(
        std::vector<std::vector<int>> &board,
        int row,
        int column,
        int minenum,
        int touchRow,
        int touchCol,
        int max_times,
        int64_t seed)
    {
        int times = 0;
        while (times < max_times)
        {
            laymine_op(board, row, column, minenum, touchRow, touchCol, seed);
            times++;
            if (is_solvable(board, touchRow, touchCol))
            {
                return true;
            }

            if (seed >= 0)
            {
                rand_value(seed);
            }
        }
        laymine_op(board, row, column, minenum, touchRow, touchCol, seed);
        return false;
    }
} // namespace Mines

namespace MinesSolver
{
    struct MSPoint
    {
        int col{0};
        int row{0};

        MSPoint() {}
        MSPoint(int _c, int _r) : col(_c), row(_r) {}
        MSPoint(const MSPoint &p) : col(p.col), row(p.row) {}
        MSPoint &operator=(const MSPoint &p)
        {
            col = p.col;
            row = p.row;
            return *this;
        }

        friend bool operator==(const MSPoint &a, const MSPoint &b) { return a.col == b.col && a.row == b.row; }
        friend bool operator!=(const MSPoint &a, const MSPoint &b) { return a.col != b.col || a.row != b.row; }
        friend bool operator<(const MSPoint &a, const MSPoint &b) { return a.row < b.row || (a.row == b.row && a.col < b.col); }
        friend bool operator>(const MSPoint &a, const MSPoint &b) { return a.row > b.row || (a.row == b.row && a.col > b.col); }
        friend bool operator<=(const MSPoint &a, const MSPoint &b) { return !operator>(a, b); }
        friend bool operator>=(const MSPoint &a, const MSPoint &b) { return !operator<(a, b); }

        MSPoint operator+(const MSPoint &p) const { return MSPoint(col + p.col, row + p.row); }
        MSPoint operator-(const MSPoint &p) const { return MSPoint(col - p.col, row - p.row); }
        MSPoint operator*(const int &scale) const { return MSPoint(col * scale, row * scale); }
        MSPoint operator/(const int &scale) const { return scale == 0 ? MSPoint(0, 0) : MSPoint(col / scale, row / scale); }

        std::string toString()
        {
            std::stringstream ss;
            ss << "(" << this->col << ", " << this->row << ")";
            return ss.str();
        }
    };

    struct NumBlock
    {
        MSPoint num_block;                   // 中心数字块
        std::vector<MSPoint> mines_blocks;   // 周围的雷块
        std::vector<MSPoint> unknown_blocks; // 周围的未知块

        std::string toString()
        {
            std::stringstream ss;
            ss << "{ " << this->num_block.toString() << "; ";
            ss << "{ ";
            for (auto &block : this->mines_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->unknown_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " } }";
            return ss.str();
        }
    };

    // 求解结果
    struct SolveResult
    {
        SolveResult() {}
        virtual ~SolveResult() {}

        int type = 0; // 0: 随便结果； 1: 单点分析结果； 2: 多点分析结果

        std::vector<MSPoint> res_mines_blocks;   // 求解周围地雷块
        std::vector<MSPoint> res_safe_blocks;    // 求解周围安全块
        std::vector<MSPoint> error_mines_blocks; // 标记错误的雷块

        virtual void clear()
        {
            res_mines_blocks.clear();
            res_safe_blocks.clear();
            error_mines_blocks.clear();
        }

        virtual std::string toString()
        {
            std::stringstream ss;
            ss << "{ " << this->type << "; ";
            ss << "{ ";
            for (auto &block : this->res_mines_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->res_safe_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->error_mines_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " } }";
            return ss.str();
        }
    };

    /**
     * @brief 单点分析结果
     *
     * 在九宫中，共有X个雷，已知M个雷，N个未知块。
     *
     *  M   b
     *  a X M
     *
     * 如果 M + N == X，则N均为雷。故a b为雷
     * 如果 M == x，则N均不为雷。故a b不为雷
     */
    struct SolveDirect : public SolveResult
    {
        SolveDirect() { type = 1; }
        virtual ~SolveDirect() {}

        int num = 0;
        MSPoint num_block; // 中心数字块

        std::vector<MSPoint> init_mines_blocks;   // 初始雷块
        std::vector<MSPoint> init_unknown_blocks; // 初始未知块

        virtual void clear()
        {
            SolveResult::clear();

            num = 0;

            num_block.col = 0;
            num_block.row = 0;

            init_mines_blocks.clear();
            init_unknown_blocks.clear();
        }

        virtual std::string toString()
        {
            std::stringstream ss;
            ss << SolveResult::toString() << "----";
            ss << "{ " << this->num << "; " << this->num_block.toString() << "; ";
            ss << "{ ";
            for (auto &block : this->init_mines_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->init_unknown_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " } } }";
            return ss.str();
        }
    };

    /**
     * @brief 双点分析结果
     *
     * 在两个九宫中，分别有X和Y个雷，分别已知Mx个雷和Nx个未知块，My个雷和Ny个未知块
     *
     *  a b c M
     *  M X Y d
     *
     * 如果 X - Mx - (Y - My) = Nx - Intersect(Nx, Ny)，则Nx - Intersect(Nx, Ny)为雷，Ny - Intersect(Nx, Ny)不为雷。故a为雷，d不为雷
     */
    struct SolveMinus : public SolveResult
    {
        SolveMinus() { type = 2; }
        virtual ~SolveMinus() {}

        int num_x = 0;
        int num_y = 0;

        MSPoint num_block_x; // X数字块
        MSPoint num_block_y; // Y数字块

        std::vector<MSPoint> init_mines_blocks_x;   // X初始雷块
        std::vector<MSPoint> init_unknown_blocks_x; // X初始未知块

        std::vector<MSPoint> init_mines_blocks_y;   // Y初始雷块
        std::vector<MSPoint> init_unknown_blocks_y; // Y初始未知块

        std::vector<MSPoint> init_intersect_blocks; // 初始相交的未知块

        virtual void clear()
        {
            SolveResult::clear();

            num_x = 0;
            num_y = 0;

            num_block_x.col = 0;
            num_block_x.row = 0;
            num_block_y.col = 0;
            num_block_y.row = 0;

            init_mines_blocks_x.clear();
            init_unknown_blocks_x.clear();
            init_mines_blocks_y.clear();
            init_unknown_blocks_y.clear();
            init_intersect_blocks.clear();
        }

        virtual std::string toString()
        {
            std::stringstream ss;
            ss << SolveResult::toString() << "----";
            ss << "{ " << num_x << "; " << this->num_block_x.toString() << "; ";
            ss << "{ ";
            for (auto &block : this->init_mines_blocks_x)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->init_unknown_blocks_x)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ " << num_y << "; " << this->num_block_y.toString() << "; ";
            ss << "{ ";
            for (auto &block : this->init_mines_blocks_y)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->init_unknown_blocks_y)
            {
                ss << block.toString() << ",";
            }
            ss << " }; ";
            ss << "{ ";
            for (auto &block : this->init_intersect_blocks)
            {
                ss << block.toString() << ",";
            }
            ss << " } } }";
            return ss.str();
        }
    };

    /**
     * @brief 检查是否胜利
     *
     * @param board_of_game
     * @param total_mines   所有炸弹的位置
     * @return true
     * @return false
     */
    bool isVictory(const std::vector<std::vector<int>> &board_of_game, const std::vector<MSPoint> &total_mines)
    {
        bool isVictory = true;
        for (const auto &mine : total_mines)
        {
            if (board_of_game[mine.row][mine.col] != 11)
            {
                isVictory = false;
                break;
            }
        }

        return isVictory;
    }

    /**
     * @brief 校验地图
     *
     * @param board
     * @param board_of_game
     * @param total_mines
     */
    void check_board(const std::vector<std::vector<int>> &board, std::vector<std::vector<int>> &board_of_game)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();

        for (int r = 0; r < board_row; r++)
        {
            for (int c = 0; c < board_col; c++)
            {
                // 如果地图标雷，但是标错了，需要还原成未知状态
                if (board_of_game[r][c] == 11 && board[r][c] != -1)
                {
                    board_of_game[r][c] = 10;
                }
            }
        }
    }

    /**
     * @brief 校验地图
     *
     * @param board
     * @param board_of_game
     * @param total_mines
     */
    void check_board(const std::vector<std::vector<int>> &board, std::vector<std::vector<int>> &board_of_game, std::vector<MSPoint> &error_mines_blocks)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();

        error_mines_blocks.clear();

        for (int r = 0; r < board_row; r++)
        {
            for (int c = 0; c < board_col; c++)
            {
                // 如果地图标雷，但是标错了，需要还原成未知状态
                if (board_of_game[r][c] == 11 && board[r][c] != -1)
                {
                    board_of_game[r][c] = 10;
                    error_mines_blocks.push_back(MSPoint(c, r));
                }
            }
        }
    }

    /**
     * @brief 获得所有地雷
     *
     * @param board
     * @param total_mines
     */
    void check_total_mines(const std::vector<std::vector<int>> &board, std::vector<MSPoint> &total_mines)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();

        total_mines.clear();

        for (int r = 0; r < board_row; r++)
        {
            for (int c = 0; c < board_col; c++)
            {
                if (board[r][c] == -1)
                {
                    total_mines.push_back(MSPoint(c, r));
                }
            }
        }
    }

    /**
     * @brief 点开地图
     *
     * @param board
     * @param board_of_game
     * @param open_pos
     * @return bool true：开启正常 false：开启失败
     */
    bool open_board(const std::vector<std::vector<int>> &board, std::vector<std::vector<int>> &board_of_game, std::vector<MSPoint> &open_pos)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();

        while (!open_pos.empty())
        {
            auto top = open_pos.back();
            open_pos.pop_back();

            if (board[top.row][top.col] > 0)
            {
                board_of_game[top.row][top.col] = board[top.row][top.col];
            }
            else if (board[top.row][top.col] == 0)
            {
                board_of_game[top.row][top.col] = 0;
                for (int r = std::max(1, top.row) - 1; r < std::min(board_row, top.row + 2); r++)
                {
                    for (int c = std::max(1, top.col) - 1; c < std::min(board_col, top.col + 2); c++)
                    {
                        if ((top.row != r || top.col != c) && (board_of_game[r][c] == 10 || board_of_game[r][c] == 12))
                        {
                            open_pos.push_back(MSPoint(c, r));
                        }
                    }
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief 单点求解
     *
     * 在九宫中，共有X个雷，已知M个雷，N个未知块。
     *
     *  M   b
     *  a X M
     *
     * 如果 M + N == X，则N均为雷。故a b为雷
     * 如果 M == x，则N均不为雷。故a b不为雷
     *
     * @param board_of_game
     * @param num_blocks
     * @param res_mines_blocks
     * @param res_safe_blocks
     */
    void solve_by_direct(std::vector<std::vector<int>> &board_of_game, std::vector<NumBlock> &num_blocks, std::vector<MSPoint> &res_mines_blocks, std::vector<MSPoint> &res_safe_blocks)
    {
        res_mines_blocks.clear();
        res_safe_blocks.clear();

        for (auto it = num_blocks.begin(); it != num_blocks.end();)
        {
            int row = it->num_block.row;
            int col = it->num_block.col;

            // 检查是否可以进行分析
            for (auto it_unknown = it->unknown_blocks.begin(); it_unknown != it->unknown_blocks.end();)
            {
                int unknown_value = board_of_game[it_unknown->row][it_unknown->col];
                if (unknown_value == 11)
                {
                    it->mines_blocks.push_back(*it_unknown);
                    it_unknown = it->unknown_blocks.erase(it_unknown);
                    continue;
                }
                else if (unknown_value != 10)
                {
                    it_unknown = it->unknown_blocks.erase(it_unknown);
                    continue;
                }
                ++it_unknown;
            }

            // 没有可以分析的的安全块
            if (it->unknown_blocks.empty())
            {
                it = num_blocks.erase(it);
                continue;
            }

            int total_mines = board_of_game[row][col];
            int mines_num = it->mines_blocks.size();
            int unknown_num = it->unknown_blocks.size();

            if (total_mines == mines_num)
            {
                // 剩余的未知块都是都安全块
                for (auto it_unknown = it->unknown_blocks.begin(); it_unknown != it->unknown_blocks.end();)
                {
                    board_of_game[it_unknown->row][it_unknown->col] = 12;
                    res_safe_blocks.push_back(*it_unknown);
                    it_unknown = it->unknown_blocks.erase(it_unknown);
                }
            }
            else if (total_mines == unknown_num + mines_num)
            {
                // 剩余的未知块都是雷
                for (auto it_unknown = it->unknown_blocks.begin(); it_unknown != it->unknown_blocks.end();)
                {
                    board_of_game[it_unknown->row][it_unknown->col] = 11;
                    res_mines_blocks.push_back(*it_unknown);
                    it_unknown = it->unknown_blocks.erase(it_unknown);
                }
            }

            if (it->unknown_blocks.empty())
            {
                it = num_blocks.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    /**
     * @brief 双点求解
     *
     * 在两个九宫中，分别有X和Y个雷，分别已知Mx个雷和Nx个未知块，My个雷和Ny个未知块
     *
     *  a b c M
     *  M X Y d
     *
     * 如果 X - Mx - (Y - My) = Nx - Intersect(Nx, Ny)，则Nx - Intersect(Nx, Ny)为雷，Ny - Intersect(Nx, Ny)不为雷。故a为雷，d不为雷
     *
     * @param board_of_game
     * @param num_blocks
     * @param res_mines_blocks
     * @param res_safe_blocks
     */
    void solve_by_minus(std::vector<std::vector<int>> &board_of_game, std::vector<NumBlock> &num_blocks, std::vector<MSPoint> &res_mines_blocks, std::vector<MSPoint> &res_safe_blocks)
    {
        res_mines_blocks.clear();
        res_safe_blocks.clear();

        for (int idx_a = 0; idx_a < static_cast<int>(num_blocks.size());)
        {
            int row_a = num_blocks[idx_a].num_block.row;
            int col_a = num_blocks[idx_a].num_block.col;

            // 检查是否可以进行分析
            for (auto it_unknown = num_blocks[idx_a].unknown_blocks.begin(); it_unknown != num_blocks[idx_a].unknown_blocks.end();)
            {
                int unknown_value = board_of_game[it_unknown->row][it_unknown->col];
                if (unknown_value == 11)
                {
                    num_blocks[idx_a].mines_blocks.push_back(*it_unknown);
                    it_unknown = num_blocks[idx_a].unknown_blocks.erase(it_unknown);
                    continue;
                }
                else if (unknown_value != 10)
                {
                    it_unknown = num_blocks[idx_a].unknown_blocks.erase(it_unknown);
                    continue;
                }
                ++it_unknown;
            }

            // 没有可以分析的的安全块
            if (num_blocks[idx_a].unknown_blocks.empty())
            {
                num_blocks.erase(num_blocks.begin() + idx_a);
                continue;
            }

            bool is_delete_a = false; // 是否删除a

            for (int idx_b = idx_a + 1; idx_b < static_cast<int>(num_blocks.size());)
            {
                int col_b = num_blocks[idx_b].num_block.col;
                int row_b = num_blocks[idx_b].num_block.row;

                // b于a相交在2格之内
                if (col_b < col_a - 2 || col_b > col_a + 2 || row_b < row_a - 2 || row_b > row_a + 2)
                {
                    idx_b++;
                    continue;
                }

                // 检查是否可以进行分析
                for (auto it_safe = num_blocks[idx_b].unknown_blocks.begin(); it_safe != num_blocks[idx_b].unknown_blocks.end();)
                {
                    int safe_value = board_of_game[it_safe->row][it_safe->col];
                    if (safe_value == 11)
                    {
                        num_blocks[idx_b].mines_blocks.push_back(*it_safe);
                        it_safe = num_blocks[idx_b].unknown_blocks.erase(it_safe);
                        continue;
                    }
                    else if (safe_value != 10)
                    {
                        it_safe = num_blocks[idx_b].unknown_blocks.erase(it_safe);
                        continue;
                    }
                    ++it_safe;
                }

                // 没有可以分析的的未知块
                if (num_blocks[idx_b].unknown_blocks.empty())
                {
                    num_blocks.erase(num_blocks.begin() + idx_b);
                    continue;
                }

                std::vector<MSPoint> intersect_blocks; // 相交部分
                std::set_intersection(num_blocks[idx_a].unknown_blocks.begin(), num_blocks[idx_a].unknown_blocks.end(), num_blocks[idx_b].unknown_blocks.begin(), num_blocks[idx_b].unknown_blocks.end(), std::back_inserter(intersect_blocks));

                // 不相交
                if (intersect_blocks.empty())
                {
                    idx_b++;
                    continue;
                }

                int total_mines_a = board_of_game[row_a][col_a];             // a的总雷
                int mines_num_a = num_blocks[idx_a].mines_blocks.size();     // a的已知雷
                int unknown_num_a = num_blocks[idx_a].unknown_blocks.size(); // a的未知块

                int total_mines_b = board_of_game[row_b][col_b];            // b的总雷
                int mines_num_b = num_blocks[idx_b].mines_blocks.size();    // b的已知雷
                int unknow_num_b = num_blocks[idx_b].unknown_blocks.size(); // b的未知块

                int intersect_num = intersect_blocks.size(); // 相交的未知块

                // 分区XY
                int index_x = -1;
                int index_y = -1;
                do
                {
                    if (total_mines_a - mines_num_a >= total_mines_b - mines_num_b)
                    {
                        int left = total_mines_a - mines_num_a - (total_mines_b - mines_num_b);
                        int right = unknown_num_a - intersect_num;

                        if (left == right)
                        {
                            index_x = idx_a;
                            index_y = idx_b;
                            break;
                        }
                    }

                    if (total_mines_b - mines_num_b >= total_mines_a - mines_num_a)
                    {
                        int left = total_mines_b - mines_num_b - (total_mines_a - mines_num_a);
                        int right = unknow_num_b - intersect_num;

                        if (left == right)
                        {
                            index_x = idx_b;
                            index_y = idx_a;
                            break;
                        }
                    }
                } while (false);

                if (index_x >= 0 && index_y >= 0)
                {
                    // X 中不在公共区域的均为雷
                    for (auto it_unknown = num_blocks[index_x].unknown_blocks.begin(); it_unknown != num_blocks[index_x].unknown_blocks.end();)
                    {
                        if (std::find(intersect_blocks.begin(), intersect_blocks.end(), *it_unknown) == intersect_blocks.end())
                        {
                            board_of_game[it_unknown->row][it_unknown->col] = 11;
                            res_mines_blocks.push_back(*it_unknown);
                            num_blocks[index_x].mines_blocks.push_back(*it_unknown);
                            it_unknown = num_blocks[index_x].unknown_blocks.erase(it_unknown);
                        }
                        else
                        {
                            ++it_unknown;
                        }
                    }

                    // Y 中不在公共区域的均不为雷
                    for (auto it_unknown = num_blocks[index_y].unknown_blocks.begin(); it_unknown != num_blocks[index_y].unknown_blocks.end();)
                    {
                        if (std::find(intersect_blocks.begin(), intersect_blocks.end(), *it_unknown) == intersect_blocks.end())
                        {
                            board_of_game[it_unknown->row][it_unknown->col] = 12;
                            res_safe_blocks.push_back(*it_unknown);
                            it_unknown = num_blocks[index_y].unknown_blocks.erase(it_unknown);
                        }
                        else
                        {
                            ++it_unknown;
                        }
                    }
                }

                if (num_blocks[idx_b].unknown_blocks.empty())
                {
                    num_blocks.erase(num_blocks.begin() + idx_b);
                }
                else
                {
                    idx_b++;
                }

                if (num_blocks[idx_a].unknown_blocks.empty())
                {
                    num_blocks.erase(num_blocks.begin() + idx_a);
                    is_delete_a = true;
                    break;
                }
            }

            if (!is_delete_a)
            {
                idx_a++;
            }
        }
    }

    /**
     * @brief 筛选周围存在未知块的数字块
     *
     * @param board_of_game
     * @param num_block
     */
    void filter_num_block(const std::vector<std::vector<int>> &board_of_game, std::vector<NumBlock> &num_blocks)
    {
        const int board_row = board_of_game.size();
        const int board_col = board_of_game[0].size();

        NumBlock block;

        num_blocks.clear();
        for (int row = 0; row < board_row; row++)
        {
            for (int col = 0; col < board_col; col++)
            {
                if (board_of_game[row][col] > 0 && board_of_game[row][col] < 10)
                {
                    block.num_block.col = col;
                    block.num_block.row = row;

                    for (int n_r = std::max(1, row) - 1; n_r < std::min(board_row, row + 2); n_r++)
                    {
                        for (int n_c = std::max(1, col) - 1; n_c < std::min(board_col, col + 2); n_c++)
                        {
                            if (board_of_game[n_r][n_c] == 10)
                            {
                                block.unknown_blocks.push_back(MSPoint(n_c, n_r));
                            }
                            else if (board_of_game[n_r][n_c] == 11)
                            {
                                block.mines_blocks.push_back(MSPoint(n_c, n_r));
                            }
                        }
                    }

                    if (!block.unknown_blocks.empty())
                    {
                        // 排序便于求交集
                        std::sort(block.unknown_blocks.begin(), block.unknown_blocks.end());
                        std::sort(block.mines_blocks.begin(), block.mines_blocks.end());
                        num_blocks.push_back(block);
                    }

                    block.num_block.col = 0;
                    block.num_block.row = 0;
                    block.mines_blocks.clear();
                    block.unknown_blocks.clear();
                }
            }
        }
    }

    /**
     * @brief 求解 游戏地图 0~8：雷的数量 10:未知 11:标记为雷 12:标记为安全块 -1:确定为雷
     *
     * @param board
     * @param touch_row
     * @param touch_col
     * @return true
     * @return false
     */
    bool is_solvable(const std::vector<std::vector<int>> &board, int touch_row, int touch_col)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();
        bool isSolve = true;              // 是否解决
        std::vector<MSPoint> total_mines; // 所有炸弹的位置

        std::vector<std::vector<int>> board_of_game(board_row, std::vector<int>(board_col, 10));

        std::vector<MSPoint> touch_blocks = {MSPoint(touch_col, touch_row)};
        if (!open_board(board, board_of_game, touch_blocks))
        {
            return false;
        }

        // 校验地图
        check_board(board, board_of_game);

        // 获得地雷
        check_total_mines(board_of_game, total_mines);

        while (!isVictory(board_of_game, total_mines))
        {
            std::vector<NumBlock> num_blocks; // 存在未知块的数字块
            filter_num_block(board_of_game, num_blocks);

            std::vector<MSPoint> mines_blocks; // 查找的雷
            std::vector<MSPoint> safe_blocks;  // 查找的安全块

            // 单点分析
            solve_by_direct(board_of_game, num_blocks, mines_blocks, safe_blocks);
            if (safe_blocks.empty() && mines_blocks.empty())
            {
                // 多点分析
                solve_by_minus(board_of_game, num_blocks, mines_blocks, safe_blocks);
                if (safe_blocks.empty() && mines_blocks.empty())
                {
                    isSolve = false;
                    break;
                }
            }

            if (!open_board(board, board_of_game, safe_blocks))
            {
                isSolve = false;
                break;
            }
        }

        return isSolve;
    }

    /**
     * @brief 单点分析 优先求解直接可以推出的地雷，其次求解空白块。找到一个便返回
     *
     * @param board_of_game
     * @param num_blocks
     * @param st_solve_direct
     */
    void solve_one_by_direct(const std::vector<std::vector<int>> &board_of_game, const std::vector<NumBlock> &num_blocks, SolveDirect &st_solve_direct)
    {
        SolveDirect st_no_mines_solve_direct; // 没有找到炸弹的求解

        st_solve_direct.clear();

        for (auto it = num_blocks.begin(); it != num_blocks.end(); ++it)
        {
            int row = it->num_block.row;
            int col = it->num_block.col;

            int total_mines = board_of_game[row][col];
            int mines_num = it->mines_blocks.size();
            int unknown_num = it->unknown_blocks.size();

            std::vector<MSPoint> res_mines_blocks;
            std::vector<MSPoint> res_safe_blocks;

            if (total_mines == mines_num)
            {
                // 剩余的未知块都是安全块
                for (auto it_unknown = it->unknown_blocks.begin(); it_unknown != it->unknown_blocks.end(); ++it_unknown)
                {
                    res_safe_blocks.push_back(*it_unknown);
                }
            }
            else if (total_mines == unknown_num + mines_num)
            {
                // 剩余的未知块都是雷
                for (auto it_unknown = it->unknown_blocks.begin(); it_unknown != it->unknown_blocks.end(); ++it_unknown)
                {
                    res_mines_blocks.push_back(*it_unknown);
                }
            }

            // 找到一个有雷的情况便直接退出
            if (!res_mines_blocks.empty())
            {
                st_solve_direct.num = board_of_game[row][col];
                st_solve_direct.num_block = it->num_block;
                st_solve_direct.init_mines_blocks = it->mines_blocks;
                st_solve_direct.init_unknown_blocks = it->unknown_blocks;
                st_solve_direct.res_mines_blocks = res_mines_blocks;
                st_solve_direct.res_safe_blocks = res_safe_blocks;
                break;
            }

            // 找到一个有安全块的情况
            if (!res_safe_blocks.empty() && st_no_mines_solve_direct.res_mines_blocks.empty() && st_no_mines_solve_direct.res_safe_blocks.empty())
            {
                st_no_mines_solve_direct.num = board_of_game[row][col];
                st_no_mines_solve_direct.num_block = it->num_block;
                st_no_mines_solve_direct.init_mines_blocks = it->mines_blocks;
                st_no_mines_solve_direct.init_unknown_blocks = it->unknown_blocks;
                st_no_mines_solve_direct.res_mines_blocks = res_mines_blocks;
                st_no_mines_solve_direct.res_safe_blocks = res_safe_blocks;
            }
        }

        // 没有找到有炸弹的情况，则尝试寻找只有空白块的情况
        if (st_solve_direct.res_mines_blocks.empty() && st_solve_direct.res_safe_blocks.empty() && (!st_no_mines_solve_direct.res_mines_blocks.empty() || !st_no_mines_solve_direct.res_safe_blocks.empty()))
        {
            st_solve_direct = st_no_mines_solve_direct;
        }
    }

    /**
     * @brief 双点分析 优先求解直接可以推出的地雷，其次求解空白块。找到一个便返回
     *
     * @param board_of_game
     * @param num_blocks
     * @param st_solve_minus
     */
    void solve_one_by_minus(const std::vector<std::vector<int>> &board_of_game, const std::vector<NumBlock> &num_blocks, SolveMinus &st_solve_minus)
    {
        SolveMinus st_no_mines_solve_minus;

        st_solve_minus.clear();

        for (int idx_a = 0; idx_a < static_cast<int>(num_blocks.size()); ++idx_a)
        {
            int row_a = num_blocks[idx_a].num_block.row;
            int col_a = num_blocks[idx_a].num_block.col;

            for (int idx_b = idx_a + 1; idx_b < static_cast<int>(num_blocks.size()); ++idx_b)
            {
                int col_b = num_blocks[idx_b].num_block.col;
                int row_b = num_blocks[idx_b].num_block.row;

                // b于a相交在2格之内
                if (col_b < col_a - 2 || col_b > col_a + 2 || row_b < row_a - 2 || row_b > row_a + 2)
                {
                    continue;
                }

                std::vector<MSPoint> intersect_blocks; // 相交部分
                std::set_intersection(num_blocks[idx_a].unknown_blocks.begin(), num_blocks[idx_a].unknown_blocks.end(), num_blocks[idx_b].unknown_blocks.begin(), num_blocks[idx_b].unknown_blocks.end(), std::back_inserter(intersect_blocks));

                // 不相交
                if (intersect_blocks.empty())
                {
                    continue;
                }

                int total_mines_a = board_of_game[row_a][col_a];             // a的总雷
                int mines_num_a = num_blocks[idx_a].mines_blocks.size();     // a的已知雷
                int unknown_num_a = num_blocks[idx_a].unknown_blocks.size(); // a的未知块

                int total_mines_b = board_of_game[row_b][col_b];            // b的总雷
                int mines_num_b = num_blocks[idx_b].mines_blocks.size();    // b的已知雷
                int unknow_num_b = num_blocks[idx_b].unknown_blocks.size(); // b的未知块

                int intersect_num = intersect_blocks.size(); // 相交的未知块

                // 分区XY
                int index_x = -1;
                int index_y = -1;
                do
                {
                    if (total_mines_a - mines_num_a >= total_mines_b - mines_num_b)
                    {
                        int left = total_mines_a - mines_num_a - (total_mines_b - mines_num_b);
                        int right = unknown_num_a - intersect_num;

                        if (left == right)
                        {
                            index_x = idx_a;
                            index_y = idx_b;
                            break;
                        }
                    }

                    if (total_mines_b - mines_num_b >= total_mines_a - mines_num_a)
                    {
                        int left = total_mines_b - mines_num_b - (total_mines_a - mines_num_a);
                        int right = unknow_num_b - intersect_num;

                        if (left == right)
                        {
                            index_x = idx_b;
                            index_y = idx_a;
                            break;
                        }
                    }
                } while (false);

                std::vector<MSPoint> res_mines_blocks;
                std::vector<MSPoint> res_safe_blocks;

                if (index_x >= 0 && index_y >= 0)
                {
                    // X 中不在公共区域的均为雷
                    for (auto it_unknown = num_blocks[index_x].unknown_blocks.begin(); it_unknown != num_blocks[index_x].unknown_blocks.end(); ++it_unknown)
                    {
                        if (std::find(intersect_blocks.begin(), intersect_blocks.end(), *it_unknown) == intersect_blocks.end())
                        {
                            res_mines_blocks.push_back(*it_unknown);
                        }
                    }

                    // Y 中不在公共区域的均不为雷
                    for (auto it_unknown = num_blocks[index_y].unknown_blocks.begin(); it_unknown != num_blocks[index_y].unknown_blocks.end(); ++it_unknown)
                    {
                        if (std::find(intersect_blocks.begin(), intersect_blocks.end(), *it_unknown) == intersect_blocks.end())
                        {
                            res_safe_blocks.push_back(*it_unknown);
                        }
                    }
                }

                if (!res_mines_blocks.empty())
                {
                    st_solve_minus.num_x = board_of_game[num_blocks[index_x].num_block.row][num_blocks[index_x].num_block.col];
                    st_solve_minus.num_block_x = num_blocks[index_x].num_block;
                    st_solve_minus.init_mines_blocks_x = num_blocks[index_x].mines_blocks;
                    st_solve_minus.init_unknown_blocks_x = num_blocks[index_x].unknown_blocks;

                    st_solve_minus.num_y = board_of_game[num_blocks[index_y].num_block.row][num_blocks[index_y].num_block.col];
                    st_solve_minus.num_block_y = num_blocks[index_y].num_block;
                    st_solve_minus.init_mines_blocks_y = num_blocks[index_y].mines_blocks;
                    st_solve_minus.init_unknown_blocks_y = num_blocks[index_y].unknown_blocks;

                    st_solve_minus.init_intersect_blocks = intersect_blocks;
                    st_solve_minus.res_mines_blocks = res_mines_blocks;
                    st_solve_minus.res_safe_blocks = res_safe_blocks;

                    break;
                }

                if (!res_safe_blocks.empty() && st_no_mines_solve_minus.res_mines_blocks.empty() && st_no_mines_solve_minus.res_safe_blocks.empty())
                {
                    st_no_mines_solve_minus.num_x = board_of_game[num_blocks[index_x].num_block.row][num_blocks[index_x].num_block.col];
                    st_no_mines_solve_minus.num_block_x = num_blocks[index_x].num_block;
                    st_no_mines_solve_minus.init_mines_blocks_x = num_blocks[index_x].mines_blocks;
                    st_no_mines_solve_minus.init_unknown_blocks_x = num_blocks[index_x].unknown_blocks;

                    st_no_mines_solve_minus.num_y = board_of_game[num_blocks[index_y].num_block.row][num_blocks[index_y].num_block.col];
                    st_no_mines_solve_minus.num_block_y = num_blocks[index_y].num_block;
                    st_no_mines_solve_minus.init_mines_blocks_y = num_blocks[index_y].mines_blocks;
                    st_no_mines_solve_minus.init_unknown_blocks_y = num_blocks[index_y].unknown_blocks;

                    st_no_mines_solve_minus.init_intersect_blocks = intersect_blocks;
                    st_no_mines_solve_minus.res_mines_blocks = res_mines_blocks;
                    st_no_mines_solve_minus.res_safe_blocks = res_safe_blocks;
                }
            }

            if (!st_solve_minus.res_mines_blocks.empty())
            {
                break;
            }
        }

        // 没有找到有炸弹的情况，则尝试寻找只有空白块的情况
        if (st_solve_minus.res_mines_blocks.empty() && st_solve_minus.res_safe_blocks.empty() && (!st_no_mines_solve_minus.res_mines_blocks.empty() || !st_no_mines_solve_minus.res_safe_blocks.empty()))
        {
            st_solve_minus = st_no_mines_solve_minus;
        }
    }

    /**
     * @brief 获得求解结果 游戏地图 0~8：雷的数量 10:未知 11:标记为雷 12:标记为安全块 -1:确定为雷
     *
     * @param board
     * @param board_of_game
     * @return std::shared_ptr<SolveResult>
     */
    std::shared_ptr<SolveResult> calc_solve_result(const std::vector<std::vector<int>> &board, std::vector<std::vector<int>> &board_of_game)
    {
        const int board_row = board.size();
        const int board_col = board[0].size();

        // 校验地图
        std::vector<MSPoint> error_mines_blocks;
        check_board(board, board_of_game, error_mines_blocks);

        std::vector<NumBlock> num_blocks; // 存在未知块的数字块
        filter_num_block(board_of_game, num_blocks);

        std::shared_ptr<SolveResult> sp_solve = nullptr; // 求解结果
        do
        {
            // 单点分析
            SolveDirect st_solve_direct;
            solve_one_by_direct(board_of_game, num_blocks, st_solve_direct);
            if (!st_solve_direct.res_mines_blocks.empty() || !st_solve_direct.res_safe_blocks.empty())
            {
                sp_solve = std::make_shared<SolveDirect>(st_solve_direct);
                sp_solve->type = 1;
                break;
            }

            // 多点分析
            SolveMinus st_solve_minus;
            solve_one_by_minus(board_of_game, num_blocks, st_solve_minus);
            if (!st_solve_minus.res_mines_blocks.empty() || !st_solve_minus.res_safe_blocks.empty())
            {
                sp_solve = std::make_shared<SolveMinus>(st_solve_minus);
                sp_solve->type = 2;
                break;
            }

            std::vector<MSPoint> res_mines_blocks;
            std::vector<MSPoint> res_safe_blocks;

            // 若上述出错，则此处保底分配
            do
            {
                // 随便寻找一个数字块进行分析
                for (const auto &num_block : num_blocks)
                {
                    for (auto &unknown_block : num_block.unknown_blocks)
                    {
                        // 进行二次校验
                        if (board_of_game[unknown_block.row][unknown_block.col] == 10)
                        {
                            if (board[unknown_block.row][unknown_block.col] == -1)
                            {
                                res_mines_blocks.push_back(unknown_block);
                            }
                            else if (board[unknown_block.row][unknown_block.col] < 10)
                            {
                                res_safe_blocks.push_back(unknown_block);
                            }
                        }
                    }

                    if (!res_mines_blocks.empty() || !res_safe_blocks.empty())
                    {
                        break;
                    }
                }

                if (!res_mines_blocks.empty() || !res_safe_blocks.empty())
                {
                    break;
                }

                // 没有可分析的数字块，则寻找安全块
                for (int row = 0; row < board_row; row++)
                {
                    for (int col = 0; col < board_col; col++)
                    {
                        if (board_of_game[row][col] == 10 && board[row][col] >= 0 && board[row][col] < 10)
                        {
                            res_safe_blocks.push_back(MSPoint(row, col));
                            break;
                        }
                    }

                    if (!res_safe_blocks.empty())
                    {
                        break;
                    }
                }

                if (!res_mines_blocks.empty() || !res_safe_blocks.empty())
                {
                    break;
                }

                // 没有安全块，则直接寻找雷
                for (int row = 0; row < board_row; row++)
                {
                    for (int col = 0; col < board_col; col++)
                    {
                        if (board_of_game[row][col] == 10 && board[row][col] == -1)
                        {
                            res_mines_blocks.push_back(MSPoint(row, col));
                            break;
                        }
                    }

                    if (!res_mines_blocks.empty())
                    {
                        break;
                    }
                }

                if (!res_mines_blocks.empty() || !res_safe_blocks.empty())
                {
                    break;
                }
            } while (false);

            if (!res_mines_blocks.empty() || !res_safe_blocks.empty())
            {
                sp_solve = std::make_shared<SolveResult>();
                sp_solve->type = 0;
                sp_solve->res_mines_blocks = res_mines_blocks;
                sp_solve->res_safe_blocks = res_safe_blocks;
                break;
            }

        } while (false);

        if (sp_solve != nullptr && sp_solve->res_mines_blocks.empty() && sp_solve->res_safe_blocks.empty())
        {
            sp_solve = nullptr;
        }

        if (sp_solve != nullptr && !error_mines_blocks.empty())
        {
            sp_solve->error_mines_blocks = error_mines_blocks;
        }

        return sp_solve;
    }
} // namespace MinesSolver

int main()
{
    int row = 10;
    int column = 10;
    int mine = 30;
    int touchRow = row / 2;
    int touchCol = column / 2;
    int maxtimes = 1000;
    int loop = 100;

    int success = 0;
    int failure = 0;
    for (int i = 1; i <= loop; i++)
    {
        std::vector<std::vector<int>> board;
        bool is_solve = Mines::laymine_solvable(board, row, column, mine, touchRow, touchCol, maxtimes);
        if (is_solve)
        {
            success++;
        }
        else
        {
            failure++;
        }

        if (i % 5 == 0)
        {
            std::cout << "loop ing: " << i << " success: " << success << " failure: " << failure << std::endl;
        }
    }

    std::cout << "success: " << success << " failure: " << failure << std::endl;

    return 0;
}
