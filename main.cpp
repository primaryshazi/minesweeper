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

#include <iostream>

namespace Mines
{
    int64_t g_seed = -1;

    const int ENUM_LIMIT = 55;

    double rand_value(int64_t &seed)
    {
        seed = seed * 1103515245ll + 12345ll;
        seed = seed % 0x7fffffff;
        return (seed * 1.0) / (0x7fffffff * 1.0);
    }

    int64_t shuffle_vector(std::vector<int> &v, int64_t seed)
    {
        auto cur_seed = seed;
        for (int i = 0; i < static_cast<int>(v.size()); i++)
        {
            double rv = rand_value(cur_seed);
            int j = std::floor(rv * (i + 1));
            std::swap(v[i], v[j]);
        }

        return cur_seed;
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

    bool isVictory(const std::vector<std::vector<int>> &board_of_game, const std::vector<std::vector<int>> &Board)
    {
        int row = board_of_game.size();
        int col = board_of_game[0].size();
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (board_of_game[i][j] == 10 && Board[i][j] != -1)
                {
                    return false;
                }
            }
        }
        return true;
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

    void combine(const std::vector<std::vector<int>> &MatrixA, const std::vector<std::pair<int, int>> &Matrixx,
                 std::vector<std::vector<int>> &matrixA_squeeze, std::vector<std::pair<int, int>> &matrixx_squeeze, std::vector<std::vector<int>> &pair_cells)
    {
        matrixA_squeeze = MatrixA;
        matrixx_squeeze = Matrixx;
        pair_cells.clear();

        int cells_num = matrixx_squeeze.size();
        std::vector<int> del_cells;
        for (int i = 0; i < cells_num; i++)
        {
            pair_cells.push_back({i});
            for (int j = i + 1; j < cells_num; j++)
            {
                if (std::none_of(matrixA_squeeze.begin(), matrixA_squeeze.end(), [&](const std::vector<int> &x)
                                 { return x[i] != x[j]; }))
                {
                    pair_cells[i].push_back(j);
                    del_cells.push_back(j);
                }
            }
        }
        std::sort(del_cells.begin(), del_cells.end(), std::greater<int>());
        del_cells.erase(unique(del_cells.begin(), del_cells.end()), del_cells.end());
        for (int i : del_cells)
        {
            for (auto &row : matrixA_squeeze)
            {
                row.erase(row.begin() + i);
            }
            matrixx_squeeze.erase(matrixx_squeeze.begin() + i);
            pair_cells.erase(pair_cells.begin() + i);
        }
        int cell_squeeze_num = pair_cells.size();
        for (int i = 0; i < cell_squeeze_num; i++)
        {
            int k = pair_cells[i].size();
            for (auto &row : matrixA_squeeze)
            {
                row[i] *= k;
            }
        }
    }

    std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
    cal_cell_and_equation_map(const std::vector<std::vector<int>> &matrix_a)
    {
        int cells_num = matrix_a[0].size();
        int equations_num = matrix_a.size();
        std::vector<std::vector<int>> cell_to_equation_map(cells_num);
        std::vector<std::vector<int>> equation_to_cell_map(equations_num);
        for (int i = 0; i < equations_num; i++)
        {
            for (int j = 0; j < cells_num; j++)
            {
                if (matrix_a[i][j] >= 1)
                {
                    equation_to_cell_map[i].push_back(j);
                    cell_to_equation_map[j].push_back(i);
                }
            }
        }
        return std::make_pair(cell_to_equation_map, equation_to_cell_map);
    }

    template <typename T, typename U>
    int C_query(T n, U k)
    {
        // 查表计算8以内小数字的组合数
        static std::array<std::array<int, 9>, 9> a{{{1, 0, 0, 0, 0, 0, 0, 0, 0},
                                                    {1, 1, 0, 0, 0, 0, 0, 0, 0},
                                                    {1, 2, 1, 0, 0, 0, 0, 0, 0},
                                                    {1, 3, 3, 1, 0, 0, 0, 0, 0},
                                                    {1, 4, 6, 4, 1, 0, 0, 0, 0},
                                                    {1, 5, 10, 10, 5, 1, 0, 0, 0},
                                                    {1, 6, 15, 20, 15, 6, 1, 0, 0},
                                                    {1, 7, 21, 35, 35, 21, 7, 1, 0},
                                                    {1, 8, 28, 56, 70, 56, 28, 8, 1}}};
        return a[n][k];
    }

    bool cal_table_minenum_recursion_step(
        int idx,
        int current_amount,
        std::array<std::vector<int>, 2> &table_minenum,
        std::vector<std::vector<int>> &table_cell_minenum,
        const std::vector<std::vector<int>> &matrixA_squeeze,
        const std::vector<int> &matrix_b,
        std::vector<int> &matrix_b_remain,
        const std::vector<std::vector<int>> &combination_relationship,
        const std::vector<std::vector<int>> &cell_to_equation_map,
        const std::vector<std::vector<int>> &equation_to_cell_map,
        std::vector<int> &mine_vec)
    {
        int cells_num = matrixA_squeeze[0].size();
        if (idx >= cells_num)
        {
            // 终止条件
            int total_mines_num = std::accumulate(mine_vec.begin(), mine_vec.end(), 0);
            if (total_mines_num >= static_cast<int>(table_minenum[1].size()))
            {
                return false;
            }
            table_minenum[1][total_mines_num] += current_amount;
            for (int idn = 0; idn < static_cast<int>(mine_vec.size()); idn++)
            {
                table_cell_minenum[total_mines_num][idn] += current_amount * mine_vec[idn] / combination_relationship[idn].size();
            }
            return true;
        }

        int upper_limit = combination_relationship[idx].size();
        int lower_limit = 0;
        for (auto cell_i : cell_to_equation_map[idx])
        {
            if (matrixA_squeeze[cell_i][idx] == 0)
            {
                continue;
            }
            int upper_limit_i = std::min(matrix_b_remain[cell_i], static_cast<int>(combination_relationship[idx].size()));
            int lower_limit_i = matrix_b_remain[cell_i];
            for (auto j : equation_to_cell_map[cell_i])
            {
                if (j > idx)
                {
                    lower_limit_i -= combination_relationship[j].size();
                }
            }
            if (upper_limit_i < static_cast<int>(upper_limit))
            {
                upper_limit = upper_limit_i;
            }
            if (lower_limit_i > static_cast<int>(lower_limit))
            {
                lower_limit = lower_limit_i;
            }
        }

        for (int u = lower_limit; u < upper_limit + 1; u++)
        {
            mine_vec[idx] = u;
            if (u > 0)
            {
                for (auto tt : cell_to_equation_map[idx])
                {
                    matrix_b_remain[tt] -= u;
                }
            }
            cal_table_minenum_recursion_step(
                idx + 1,
                current_amount * C_query(combination_relationship[idx].size(), u),
                table_minenum,
                table_cell_minenum,
                matrixA_squeeze,
                matrix_b,
                matrix_b_remain,
                combination_relationship,
                cell_to_equation_map,
                equation_to_cell_map,
                mine_vec);
            for (auto tt : cell_to_equation_map[idx])
            {
                matrix_b_remain[tt] += u;
            }
            mine_vec[idx] = 0;
        }
        return false;
    }

    std::pair<std::array<std::vector<int>, 2>, std::vector<std::vector<int>>> cal_table_minenum_recursion(
        const std::vector<std::vector<int>> &matrixA_squeeze,
        const std::vector<std::pair<int, int>> &matrixx_squeeze,
        const std::vector<int> &matrix_b,
        const std::vector<std::vector<int>> &combination_relationship)
    {
        // 递归算法，得到雷数分布表和每格是雷情况数表，顺便计算最小、最大雷数
        // 输入矩阵必须是非空的，且行列数必须匹配
        // 行数和列数至少为1
        int cells_num = matrixx_squeeze.size();
        if (cells_num > ENUM_LIMIT)
        {
            // 超出枚举极限长度异常
            return std::make_pair(std::array<std::vector<int>, 2>{}, std::vector<std::vector<int>>{});
        }
        int cells_num_total = std::accumulate(combination_relationship.begin(), combination_relationship.end(), 0,
                                              [](int item, const std::vector<int> &x)
                                              { return item + x.size(); });
        // cells_num_total指合并前的格子数

        bool flag_legal_board = true;
        std::array<std::vector<int>, 2> table_minenum{{std::vector<int>(cells_num_total + 1),
                                                       std::vector<int>(cells_num_total + 1, 0)}};

        auto cal_cell_and_equation_map_ans = cal_cell_and_equation_map(matrixA_squeeze);
        auto cell_to_equation_map = std::get<0>(cal_cell_and_equation_map_ans);
        auto equation_to_cell_map = std::get<1>(cal_cell_and_equation_map_ans);
        // 计算两个映射表以减少复杂度
        // std::cout << "cell_to_equation_map = " << cell_to_equation_map << "; equation_to_cell_map = " << equation_to_cell_map << std::endl;

        std::vector<std::vector<int>>
            table_cell_minenum(cells_num_total + 1, std::vector<int>(cells_num, 0));

        // std::cout << matrixA_squeeze << std::endl;

        {
            auto matrix_b_clone = matrix_b;
            auto mine_vec = std::vector<int>(cells_num, 0);
            cal_table_minenum_recursion_step(
                0,
                1,
                table_minenum,
                table_cell_minenum,
                matrixA_squeeze,
                matrix_b,
                matrix_b_clone,
                combination_relationship,
                cell_to_equation_map,
                equation_to_cell_map,
                mine_vec);
        }
        // std::cout << "table_cell_minenum" << table_cell_minenum << std::endl;
        // std::cout << "table_minenum" << table_minenum[0] << ", " << table_minenum[1] << std::endl;
        while (table_minenum[1][0] == 0)
        {
            table_minenum[0].erase(table_minenum[0].begin());
            table_minenum[1].erase(table_minenum[1].begin());
            table_cell_minenum.erase(table_cell_minenum.begin());
            if (table_cell_minenum.empty())
            {
                flag_legal_board = false;
                break;
            }
        }
        if (flag_legal_board)
        {
            while (table_minenum[1][table_cell_minenum.size() - 1] == 0)
            {
                table_minenum[0].pop_back();
                table_minenum[1].pop_back();
                table_cell_minenum.pop_back();
            }
        }
        if (flag_legal_board)
        {
            return std::make_pair(table_minenum, table_cell_minenum);
        }
        else
        {
            return std::make_pair(std::array<std::vector<int>, 2>{}, std::vector<std::vector<int>>{});
        }
    }

    std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> solve_enumerate(
        const std::vector<std::vector<std::vector<int>>> &As,
        const std::vector<std::vector<std::pair<int, int>>> &xs,
        const std::vector<std::vector<int>> &bs)
    {
        if (bs.empty())
        {
            return std::make_pair(std::vector<std::pair<int, int>>(), std::vector<std::pair<int, int>>());
        }

        std::vector<std::pair<int, int>> not_mine;
        std::vector<std::pair<int, int>> is_mine;
        int block_num = xs.size();

        std::vector<std::vector<std::vector<int>>> comb_relp_s;
        std::vector<std::vector<std::vector<int>>> matrixA_squeeze_s;
        std::vector<std::vector<std::pair<int, int>>> matrixx_squeeze_s;
        for (int i = 0; i < block_num; i++)
        {
            if (xs[i].size() > ENUM_LIMIT)
            {
                return std::make_pair(not_mine, is_mine);
            }

            std::vector<std::vector<int>> matrixA_squeeze;
            std::vector<std::pair<int, int>> matrixx_squeeze;
            std::vector<std::vector<int>> combination_relationship;
            combine(As[i], xs[i], matrixA_squeeze, matrixx_squeeze, combination_relationship);

            comb_relp_s.emplace_back(combination_relationship);
            matrixA_squeeze_s.emplace_back(matrixA_squeeze);
            matrixx_squeeze_s.emplace_back(matrixx_squeeze);
        }
        for (int i = 0; i < block_num; i++)
        {
            auto cal_table_minenum_recursion_ans = cal_table_minenum_recursion(
                matrixA_squeeze_s[i],
                matrixx_squeeze_s[i],
                bs[i],
                comb_relp_s[i]);
            auto table_minenum_i = std::get<0>(cal_table_minenum_recursion_ans);
            auto table_cell_minenum_i = std::get<1>(cal_table_minenum_recursion_ans);

            for (int jj = 0; jj < static_cast<int>(table_cell_minenum_i[0].size()); jj++)
            {
                int s_num = 0; // 该合成格子的总情况数
                for (int ii = 0; ii < static_cast<int>(table_cell_minenum_i.size()); ii++)
                {
                    s_num += table_cell_minenum_i[ii][jj];
                }
                if (s_num == 0)
                {
                    for (auto kk : comb_relp_s[i][jj])
                    {
                        not_mine.emplace_back(xs[i][kk]);
                    }
                }
                else if (s_num == std::accumulate(table_minenum_i[1].begin(), table_minenum_i[1].end(), 0))
                {
                    for (auto kk : comb_relp_s[i][jj])
                    {
                        is_mine.emplace_back(xs[i][kk]);
                    }
                }
            }
        }
        return std::make_pair(not_mine, is_mine);
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
        std::vector<std::vector<int>> board_of_game(row, std::vector<int>(column, 10));
        // 10是未打开，11是标雷
        // 局面大小必须超过6*6
        refresh_board(board, board_of_game, {{touchRow, touchCol}});
        if (isVictory(board_of_game, board))
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
                    auto solve_enumerate_ans = solve_enumerate(As, xs, bs);
                    if (std::get<0>(solve_enumerate_ans).empty() && std::get<1>(solve_enumerate_ans).empty())
                    {
                        return false;
                    }
                    else
                    {
                        not_mine = std::get<0>(solve_enumerate_ans);
                    }

                    if (!std::get<1>(solve_enumerate_ans).empty())
                    {
                        for (const auto &op : std::get<1>(solve_enumerate_ans))
                        {
                            board_of_game[op.first][op.second] = 11;
                        }
                    }
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
            if (isVictory(board_of_game, board))
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
        int touchCol)
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

        if (g_seed < 0)
        {
            std::random_device rng;
            std::shuffle(board_dim.begin(), board_dim.end(), rng);
        }
        else
        {
            g_seed = shuffle_vector(board_dim, g_seed);
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
} // namespace Mines

int main()
{
    Mines::g_seed = 123456789;

    int row = 32;
    int column = 32;
    int mine = 235;
    int touchRow = row / 2;
    int touchCol = column / 2;
    int maxtimes = 1000;
    const int loop = 1;

    int success = 0;
    int failure = 0;

    int startMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    for (int i = 1; i <= loop; i++)
    {
        std::vector<std::vector<int>> board;
        bool isSolved = Mines::laymine_solvable(board, row, column, mine, touchRow, touchCol, maxtimes);
        if (isSolved)
        {
            success++;
        }
        else
        {
            failure++;
        }

        if (i % 5 == 0)
        {
            std::cout << "run = " << i << "; success = " << success << "; failure = " << failure << std::endl;
        }

        if (loop == 1)
        {
            for (auto &ls : board)
            {
                for (auto &v : ls)
                {
                    if (v == -1)
                    {
                        v = 9;
                    }
                }
            }

            for (int r = 0; r < row; r++)
            {
                std::cout << "{";
                for (int c = 0; c < column; c++)
                {
                    std::cout << board[r][c] << ", ";
                }
                std::cout << "}," << std::endl;
            }
        }
    }
    int endMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    std::cout << "success = " << success << "; failure = " << failure << "; time = " << (endMs - startMs) / 1000 << std::endl;

    return 0;
}
