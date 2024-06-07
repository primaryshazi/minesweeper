use std::cmp::{max, min};

extern crate rand;
use crate::rand::prelude::SliceRandom;

const SHOW_DEBUG: i32 = 1;

// 最大枚举长度限制。超过这个长度，概率计算不准。全雷网的高级没有发现超出此限制的。
const ENUM_LIMIT: usize = 55;

// 随机种子
static mut g_seed: i64 = -1;

pub fn rand_value(seed: &mut i64) -> f64 {
    *seed = *seed * 1103515245i64 + 12345i64;
    *seed = *seed % (0x7fffffff as i64);
    return (*seed as f64) / (0x7fffffff as f64);
}

pub fn shuffle_vector(v: &mut Vec<i32>, seed: i64) -> i64 {
    let mut cur_seed = seed;
    for i in 0..v.len() {
        let rv = rand_value(&mut cur_seed);
        let j = (rv * ((i + 1) as f64)).floor() as usize;
        v.swap(i, j);
    }

    return cur_seed;
}

/// 用几种模板，检测实际局面中是否有明显的死猜的结构。
/// - 使用模板包括：工型、回型、器型。
/// - 注意：对于一个局面，即使该检测返回true，也不能判断其必然是无猜的局面。想要真正判断一个局面无猜，请使用[is_solvable](#is_solvable)
/// - 注意：局面至少大于4*4。
pub fn unsolvable_structure(BoardCheck: &Vec<Vec<i32>>) -> bool {
    let row = BoardCheck.len();
    let column = BoardCheck[0].len();
    let mut Board = vec![vec![0; column]; row];
    for i in 0..row {
        for j in 0..column {
            if BoardCheck[i][j] == -1 {
                Board[i][j] = -1;
            }
        }
    }
    for i in 0..row - 2 {
        // 检查左右两侧的工
        if i < row - 3 {
            if Board[i][0] == -1
                && Board[i][1] == -1
                && Board[i + 3][0] == -1
                && Board[i + 3][1] == -1
                && Board[i + 1][0] + Board[i + 2][0] == -1
                || Board[i][column - 1] == -1
                    && Board[i][column - 2] == -1
                    && Board[i + 3][column - 1] == -1
                    && Board[i + 3][column - 2] == -1
                    && Board[i + 1][column - 1] + Board[i + 2][column - 1] == -1
            {
                return true;
            }
        }
        if Board[i][2] == -1
            && Board[i + 1][2] == -1
            && Board[i + 2][2] == -1
            && Board[i + 1][0] + Board[i + 1][1] == -1
            || Board[i][column - 3] == -1
                && Board[i + 1][column - 3] == -1
                && Board[i + 2][column - 3] == -1
                && Board[i + 1][column - 1] + Board[i + 1][column - 2] == -1
            || Board[i][0] == -1
                && Board[i][1] == -1
                && Board[i + 1][1] == -1
                && Board[i + 2][1] == -1
                && Board[i + 2][0] == -1
                && Board[i + 1][0] == 0
            || Board[i][column - 1] == -1
                && Board[i][column - 2] == -1
                && Board[i + 1][column - 2] == -1
                && Board[i + 2][column - 2] == -1
                && Board[i + 2][column - 1] == -1
                && Board[i + 1][column - 1] == 0
        {
            return true;
        }
        if i < row - 3 {
            if Board[i][2] == -1
                && Board[i + 3][2] == -1
                && Board[i + 1][0] + Board[i + 1][1] == -1
                && Board[i + 1][1] + Board[i + 2][1] == -1
                && Board[i + 2][1] + Board[i + 2][0] == -1
                || Board[i][column - 3] == -1
                    && Board[i + 3][column - 3] == -1
                    && Board[i + 1][column - 1] + Board[i + 1][column - 2] == -1
                    && Board[i + 1][column - 2] + Board[i + 2][column - 2] == -1
                    && Board[i + 2][column - 2] + Board[i + 2][column - 1] == -1
            {
                return true;
            }
        }
    }
    for j in 0..column - 2 {
        // 检查上下两侧
        if j < column - 3 {
            if Board[0][j] == -1
                && Board[1][j] == -1
                && Board[0][j + 3] == -1
                && Board[1][j + 3] == -1
                && Board[0][j + 1] + Board[0][j + 2] == -1
                || Board[row - 1][j] == -1
                    && Board[row - 2][j] == -1
                    && Board[row - 1][j + 3] == -1
                    && Board[row - 2][j + 3] == -1
                    && Board[row - 1][j + 1] + Board[row - 1][j + 2] == -1
            {
                return true;
            }
        }
        if Board[2][j] == -1
            && Board[2][j + 1] == -1
            && Board[2][j + 2] == -1
            && Board[0][j + 1] + Board[1][j + 1] == -1
            || Board[row - 3][j] == -1
                && Board[row - 3][j + 1] == -1
                && Board[row - 3][j + 2] == -1
                && Board[row - 1][j + 1] + Board[row - 2][j + 1] == -1
            || Board[0][j] == -1
                && Board[1][j] == -1
                && Board[1][j + 1] == -1
                && Board[1][j + 2] == -1
                && Board[0][j + 2] == -1
                && Board[0][j + 1] == 0
            || Board[row - 1][j] == -1
                && Board[row - 2][j] == -1
                && Board[row - 2][j + 1] == -1
                && Board[row - 2][j + 2] == -1
                && Board[row - 1][j + 2] == -1
                && Board[row - 1][j + 1] == 0
        {
            return true;
        }
        if j < column - 3 {
            if Board[2][j] == -1
                && Board[2][j + 3] == -1
                && Board[0][j + 1] + Board[1][j + 1] == -1
                && Board[1][j + 1] + Board[1][j + 2] == -1
                && Board[1][j + 2] + Board[0][j + 2] == -1
                || Board[row - 3][j] == -1
                    && Board[row - 3][j + 3] == -1
                    && Board[row - 1][j + 1] + Board[row - 2][j + 1] == -1
                    && Board[row - 2][j + 1] + Board[row - 2][j + 2] == -1
                    && Board[row - 2][j + 2] + Board[row - 1][j + 2] == -1
            {
                return true;
            }
        }
    }
    if Board[0][2] == -1 && Board[1][2] == -1 && Board[0][0] + Board[0][1] == -1
        || Board[2][0] == -1 && Board[2][1] == -1 && Board[0][0] + Board[1][0] == -1
        || Board[0][column - 3] == -1
            && Board[1][column - 3] == -1
            && Board[0][column - 1] + Board[0][column - 2] == -1
        || Board[2][column - 1] == -1
            && Board[2][column - 2] == -1
            && Board[0][column - 1] + Board[1][column - 1] == -1
        || Board[row - 1][2] == -1
            && Board[row - 2][2] == -1
            && Board[row - 1][0] + Board[row - 1][1] == -1
        || Board[row - 3][0] == -1
            && Board[row - 3][1] == -1
            && Board[row - 1][0] + Board[row - 2][0] == -1
        || Board[row - 1][column - 3] == -1
            && Board[row - 2][column - 3] == -1
            && Board[row - 1][column - 1] + Board[row - 1][column - 2] == -1
        || Board[row - 3][column - 1] == -1
            && Board[row - 3][column - 2] == -1
            && Board[row - 1][column - 1] + Board[row - 2][column - 1] == -1
        || Board[0][1] + Board[1][1] + Board[1][0] == -3 && Board[0][0] == 0
        || Board[0][column - 2] + Board[1][column - 2] + Board[1][column - 1] == -3
            && Board[0][column - 1] == 0
        || Board[row - 1][column - 2] + Board[row - 2][column - 2] + Board[row - 2][column - 1]
            == -3
            && Board[row - 1][column - 1] == 0
        || Board[row - 1][1] + Board[row - 2][1] + Board[row - 2][0] == -3 && Board[row - 1][0] == 0
        || Board[2][2] == -1 && Board[0][1] + Board[1][1] == -1 && Board[1][0] + Board[1][1] == -1
        || Board[row - 3][2] == -1
            && Board[row - 1][1] + Board[row - 2][1] == -1
            && Board[row - 2][0] + Board[row - 2][1] == -1
        || Board[row - 3][column - 3] == -1
            && Board[row - 1][column - 2] + Board[row - 2][column - 2] == -1
            && Board[row - 2][column - 1] + Board[row - 2][column - 2] == -1
        || Board[2][column - 3] == -1
            && Board[0][column - 2] + Board[1][column - 2] == -1
            && Board[1][column - 1] + Board[1][column - 2] == -1
    {
        //检查四个角
        return true;
    }
    for i in 0..row - 2 {
        // 找中间的工、回、器形结构
        for j in 0..column - 2 {
            if j < column - 3 {
                if Board[i][j] == -1
                    && Board[i + 1][j] == -1
                    && Board[i + 2][j] == -1
                    && Board[i][j + 3] == -1
                    && Board[i + 1][j + 3] == -1
                    && Board[i + 2][j + 3] == -1
                    && Board[i + 1][j + 1] + Board[i + 1][j + 2] == -1
                {
                    return true;
                }
            }
            if i < row - 3 {
                if Board[i][j] == -1
                    && Board[i][j + 1] == -1
                    && Board[i][j + 2] == -1
                    && Board[i + 3][j] == -1
                    && Board[i + 3][j + 1] == -1
                    && Board[i + 3][j + 2] == -1
                    && Board[i + 1][j + 1] + Board[i + 2][j + 1] == -1
                {
                    return true;
                }
            }
            if Board[i][j] == -1
                && Board[i + 1][j] == -1
                && Board[i + 2][j] == -1
                && Board[i][j + 1] == -1
                && Board[i + 2][j + 1] == -1
                && Board[i][j + 2] == -1
                && Board[i + 1][j + 2] == -1
                && Board[i + 2][j + 2] == -1
                && Board[i + 1][j + 1] == 0
            {
                return true;
            }
            if j < column - 3 && i < row - 3 {
                if Board[i][j] == -1
                    && Board[i + 3][j] == -1
                    && Board[i][j + 3] == -1
                    && Board[i + 3][j + 3] == -1
                    && Board[i + 1][j + 1] + Board[i + 2][j + 1] == -1
                    && Board[i + 1][j + 1] + Board[i + 1][j + 2] == -1
                    && Board[i + 2][j + 1] + Board[i + 2][j + 2] == -1
                {
                    return true;
                }
            }
        }
    }
    false
}

/// 依据左击位置刷新局面。如踩雷，标上或14、15标记
/// - 注意：兼容12标记符
pub fn refresh_board(
    board: &Vec<Vec<i32>>,
    boardofGame: &mut Vec<Vec<i32>>,
    mut clicked_poses: Vec<(usize, usize)>,
) {
    let row = board.len();
    let column = board[0].len();
    // 是否踩雷
    let mut loss_flag = false;
    while let Some(top) = clicked_poses.pop() {
        let (i, j) = top;
        if board[i][j] > 0 {
            boardofGame[i][j] = board[i][j];
        } else if board[i][j] == 0 {
            boardofGame[i][j] = 0;
            for m in max(1, i) - 1..min(row, i + 2) {
                for n in max(1, j) - 1..min(column, j + 2) {
                    if (i != m || j != n) && (boardofGame[m][n] == 10 || boardofGame[m][n] == 12) {
                        clicked_poses.push((m, n));
                    }
                }
            }
        } else {
            boardofGame[i][j] = 15; // 标红雷，此处是雷，且踩到了
            loss_flag = true;
        }
    }
    // 标叉雷
    if loss_flag {
        for i in 0..row {
            for j in 0..column {
                if boardofGame[i][j] == 11 && board[i][j] != -1 {
                    boardofGame[i][j] = 14; // 叉雷，即标错的雷
                }
            }
        }
    }
}

// 判断当前是否获胜
// 游戏局面中必须没有标错的雷
// 这个函数不具备普遍意义
fn isVictory(board_of_game: &Vec<Vec<i32>>, Board: &Vec<Vec<i32>>) -> bool {
    let row = board_of_game.len();
    let col = board_of_game[0].len();
    for i in 0..row {
        for j in 0..col {
            // 胜利条件改成找出所有雷
            if board_of_game[i][j] == 10 && Board[i][j] == -1 {
                return false;
            }
        }
    }
    return true;
}

/// 根据游戏局面生成矩阵，分段。输入的必须保证是合法的游戏局面。
/// - *基于数字生成，矩阵的行可能有重复。
pub fn refresh_matrixs(
    board_of_game: &Vec<Vec<i32>>,
) -> (
    Vec<Vec<Vec<i32>>>,
    Vec<Vec<(usize, usize)>>,
    Vec<Vec<i32>>,
    usize,
    usize,
) {
    // 根据游戏局面分块生成矩阵。分段的数据结构是最外面再套一层Vec
    // board_of_game必须且肯定是正确标雷的游戏局面，但不需要标全，不能标非雷
    // 矩阵的行和列都可能有重复
    // unknow_block是未知格子数量, is_minenum是标出的是雷的数量
    let row = board_of_game.len();
    let column = board_of_game[0].len();
    let mut unknow_block = 0;
    let mut is_minenum = 0;
    let mut matrix_as = vec![];
    let mut matrix_xs = vec![];
    let mut matrix_bs = vec![];
    let mut all_cell: Vec<(usize, usize)> = vec![]; // 记录所有周围有未打开格子的数字的位置
    for i in 0..row {
        for j in 0..column {
            if board_of_game[i][j] >= 0 && board_of_game[i][j] < 10 {
                'outer: for m in max(1, i) - 1..min(row, i + 2) {
                    for n in max(1, j) - 1..min(column, j + 2) {
                        if board_of_game[m][n] == 10 {
                            all_cell.push((i, j));
                            break 'outer;
                        }
                    }
                }
            } else if board_of_game[i][j] == 10 {
                // 数内部有几个格子
                let mut flag = true;
                for m in max(1, i) - 1..min(row, i + 2) {
                    for n in max(1, j) - 1..min(column, j + 2) {
                        if board_of_game[m][n] < 10 {
                            flag = false;
                        }
                    }
                }
                if flag {
                    unknow_block += 1;
                }
            } else if board_of_game[i][j] == 11 {
                is_minenum += 1;
            }
        }
    }
    let mut p = 0; //指针，代表第几块
                   // println!("{:?}", all_cell);
    while !all_cell.is_empty() {
        matrix_xs.push(vec![]);
        matrix_bs.push(vec![]);
        let x_0 = all_cell[0].0;
        let y_0 = all_cell[0].1;
        let mut num_cells = vec![]; // 记录了当前段的数字格的坐标
        let mut temp_cells = vec![]; // 记录了待查找的数字格的坐标
        let mut flag_num = 0;
        for m in max(1, x_0) - 1..min(row, x_0 + 2) {
            for n in max(1, y_0) - 1..min(column, y_0 + 2) {
                if board_of_game[m][n] == 10 {
                    matrix_xs[p].push((m, n));
                }
                if board_of_game[m][n] == 11 {
                    flag_num += 1;
                }
            }
        }
        matrix_bs[p].push(board_of_game[x_0][y_0] - flag_num);
        num_cells.push((x_0, y_0));
        temp_cells.push((x_0, y_0));
        while !temp_cells.is_empty() {
            let x_e = temp_cells[0].0;
            let y_e = temp_cells[0].1;
            temp_cells.remove(0);
            for t in (1..all_cell.len()).rev() {
                // 从temp_cells中拿出一个格子，找出与其相邻的所有格子，加入temp_cells和matrix_bs、matrix_xs
                let x_t = all_cell[t].0;
                let y_t = all_cell[t].1;
                if x_t >= x_e + 3 || x_e >= x_t + 3 || y_t >= y_e + 3 || y_e >= y_t + 3 {
                    continue;
                }
                let mut flag_be_neighbor = false;
                for m in max(1, max(x_t, x_e)) - 1..min(row, min(x_t + 2, x_e + 2)) {
                    for n in max(1, max(y_t, y_e)) - 1..min(column, min(y_t + 2, y_e + 2)) {
                        if board_of_game[m][n] == 10 {
                            flag_be_neighbor = true;
                            break;
                        }
                    }
                } // 从局面上看，如果两个数字有相同的未知格子，那么就会分到同一块
                if flag_be_neighbor {
                    let mut flag_num = 0;
                    for m in max(1, x_t) - 1..min(row, x_t + 2) {
                        for n in max(1, y_t) - 1..min(column, y_t + 2) {
                            if board_of_game[m][n] == 10 {
                                if !matrix_xs[p].contains(&(m, n)) {
                                    matrix_xs[p].push((m, n));
                                }
                            }
                            if board_of_game[m][n] == 11 {
                                flag_num += 1;
                            }
                        }
                    }
                    matrix_bs[p].push(board_of_game[x_t][y_t] - flag_num);
                    num_cells.push((x_t, y_t));
                    temp_cells.push(all_cell[t]);
                    all_cell.remove(t);
                }
            }
        }
        matrix_as.push(vec![vec![0; matrix_xs[p].len()]; matrix_bs[p].len()]);
        for i in 0..num_cells.len() {
            for j in 0..matrix_xs[p].len() {
                if num_cells[i].0 <= matrix_xs[p][j].0 + 1
                    && matrix_xs[p][j].0 <= num_cells[i].0 + 1
                    && num_cells[i].1 <= matrix_xs[p][j].1 + 1
                    && matrix_xs[p][j].1 <= num_cells[i].1 + 1
                {
                    matrix_as[p][i][j] = 1;
                }
            }
        }
        all_cell.remove(0);
        p += 1;
    }
    (matrix_as, matrix_xs, matrix_bs, unknow_block, is_minenum)
}

// 重新分块一个矩阵
// 这些矩阵必须非空、没有空的块、没有b=0的情况
pub fn chunk_matrix(
    mut matrix_a: Vec<Vec<i32>>,
    mut matrix_x: Vec<(usize, usize)>,
    mut matrix_b: Vec<i32>,
) -> (Vec<Vec<Vec<i32>>>, Vec<Vec<(usize, usize)>>, Vec<Vec<i32>>) {
    let mut block_id = 0;
    let mut matrix_as = vec![];
    let mut matrix_xs = vec![];
    let mut matrix_bs = vec![];

    loop {
        let row_num = matrix_a.len();
        let column_num = matrix_a[0].len();
        let mut current_rows_bool = vec![false; row_num];
        let mut current_columns_bool = vec![false; column_num];
        current_columns_bool[0] = true;
        let mut column_buffer = vec![0];
        loop {
            let mut row_buffer = vec![];
            if column_buffer.is_empty() {
                break;
            }
            for i in &column_buffer {
                for idr in 0..matrix_a.len() {
                    if matrix_a[idr][*i] >= 1 && !current_rows_bool[idr] {
                        row_buffer.push(idr);
                        current_rows_bool[idr] = true;
                    }
                }
            }
            column_buffer.clear();
            if row_buffer.is_empty() {
                break;
            }
            for i in row_buffer {
                for (idc, &c) in matrix_a[i].iter().enumerate() {
                    if c >= 1 && !current_columns_bool[idc] {
                        column_buffer.push(idc);
                        current_columns_bool[idc] = true;
                    }
                }
            }
        }
        let mut current_rows = vec![];
        let mut current_columns = vec![];
        for (idx, &x) in current_columns_bool.iter().enumerate() {
            if x {
                current_columns.push(idx)
            }
        }
        for (idx, &x) in current_rows_bool.iter().enumerate() {
            if x {
                current_rows.push(idx)
            }
        }
        current_rows.sort_by(|a, b| b.cmp(a));
        current_rows.dedup();
        current_columns.sort_by(|a, b| b.cmp(a));
        current_columns.dedup();
        matrix_as.push(vec![vec![0; current_columns.len()]; current_rows.len()]);
        matrix_bs.push(vec![0; current_rows.len()]);
        matrix_xs.push(vec![(0, 0); current_columns.len()]);
        for (idx, x) in current_rows.iter().enumerate() {
            for (idy, y) in current_columns.iter().enumerate() {
                matrix_as[block_id][idx][idy] = matrix_a[*x][*y];
            }
        }
        for (idx, x) in current_rows.iter().enumerate() {
            matrix_bs[block_id][idx] = matrix_b[*x];
        }
        for (idy, y) in current_columns.iter().enumerate() {
            matrix_xs[block_id][idy] = matrix_x[*y];
        }

        if SHOW_DEBUG > 999 {
            // println!(
            //     "current_rows: {:?}\nmatrix_a: {:?}\nmatrix_b: {:?}",
            //     current_rows, matrix_a, matrix_b
            // );
            // println!();
        }

        for i in current_rows {
            matrix_a.remove(i);
            matrix_b.remove(i);
        }
        for j in current_columns {
            for k in 0..matrix_a.len() {
                matrix_a[k].remove(j);
            }
            matrix_x.remove(j);
        }

        if matrix_b.is_empty() {
            break;
        }
        block_id += 1;
    }

    if SHOW_DEBUG > 999 {
        println!(
            "matrix_as: {:?}\nmatrix_xs: {:?}\nmatrix_bs: {:?}",
            matrix_as, matrix_xs, matrix_bs
        );
        println!();
    }

    (matrix_as, matrix_xs, matrix_bs)
}

// 重新分块矩阵
// 这些矩阵必须非空、没有空的块、没有b=0的情况
pub fn chunk_matrixes(
    matrix_as: &mut Vec<Vec<Vec<i32>>>,
    matrix_xs: &mut Vec<Vec<(usize, usize)>>,
    matrix_bs: &mut Vec<Vec<i32>>,
) {
    let block_num = matrix_bs.len();
    let mut aas = vec![];
    let mut xxs = vec![];
    let mut bbs = vec![];
    for _ in 0..block_num {
        let aa = matrix_as.pop().unwrap();
        let xx = matrix_xs.pop().unwrap();
        let bb = matrix_bs.pop().unwrap();

        if SHOW_DEBUG > 999 {
            println!("aa: {:?}\nbb: {:?}\nxx: {:?}", aa, bb, xx);
            println!();
        }

        let (mut a_, mut x_, mut b_) = chunk_matrix(aa, xx, bb);
        aas.append(&mut a_);
        xxs.append(&mut x_);
        bbs.append(&mut b_);
    }
    *matrix_as = aas;
    *matrix_xs = xxs;
    *matrix_bs = bbs;
}

/// 单集合判雷引擎。
/// - 输入：3个矩阵、局面。
/// - 返回：非雷、是雷的格子，在传入的局面上标是雷（11）和非雷（12）。
/// - 返回Err(6)表示：比如数字2的周围只有1个格子没打开
/// - 注意：会维护系数矩阵、格子矩阵和数字矩阵，删、改、分段。
pub fn solve_direct(
    As: &mut Vec<Vec<Vec<i32>>>,
    xs: &mut Vec<Vec<(usize, usize)>>,
    bs: &mut Vec<Vec<i32>>,
    board_of_game: &mut Vec<Vec<i32>>,
) -> Result<(Vec<(usize, usize)>, Vec<(usize, usize)>), usize> {
    let mut is_mine = vec![];
    let mut not_mine = vec![];

    let block_num = bs.len();
    for b in (0..block_num).rev() {
        let mut matrix_column = xs[b].len();
        let mut matrix_row = bs[b].len();
        for i in (0..matrix_row).rev() {
            if As[b][i].iter().sum::<i32>() == bs[b][i] {
                for k in (0..matrix_column).rev() {
                    if As[b][i][k] >= 1 {
                        is_mine.push((xs[b][k].0, xs[b][k].1));
                        board_of_game[xs[b][k].0][xs[b][k].1] = 11;
                        xs[b].remove(k);
                        for t in 0..matrix_row {
                            bs[b][t] -= As[b][t][k];
                            As[b][t].remove(k);
                        }
                        matrix_column -= 1;
                    }
                }
                As[b].remove(i);
                bs[b].remove(i);
                matrix_row -= 1;
            }
        }
        for i in (0..matrix_row).rev() {
            if bs[b][i] == 0 {
                for k in (0..matrix_column).rev() {
                    if As[b][i][k] >= 1 {
                        not_mine.push(xs[b][k]);
                        board_of_game[xs[b][k].0][xs[b][k].1] = 12;
                        xs[b].remove(k);
                        for t in 0..matrix_row {
                            As[b][t].remove(k);
                        }
                        matrix_column -= 1;
                    }
                }
                As[b].remove(i);
                bs[b].remove(i);
                matrix_row -= 1;
            }
        }
        if bs[b].is_empty() {
            As.remove(b);
            bs.remove(b);
            xs.remove(b);
        }
    }

    if SHOW_DEBUG > 999 {
        println!("solve_direct\nAs: {:?}\nxs: {:?}\nbs: {:?}", As, xs, bs);
        println!();
    }

    let ans = bs.iter().find(|&b| match b.iter().find(|&&x| x < 0) {
        Some(_) => return true,
        None => return false,
    });
    match ans {
        Some(_) => return Err(6),
        None => {}
    }

    if SHOW_DEBUG > 999 {
        println!("solve_direct\nans: {:?}", ans);
        println!();
    }

    chunk_matrixes(As, xs, bs);
    Ok((not_mine, is_mine))
}

/// 双集合判雷引擎。
/// - 输入：3个矩阵、局面。
/// - 返回：是雷、非雷的格子，在传入的局面上标是雷（11）和非雷（12）。
/// - 注意：会维护系数矩阵、格子矩阵和数字矩阵，删、改、分段。
pub fn solve_minus(
    As: &mut Vec<Vec<Vec<i32>>>,
    xs: &mut Vec<Vec<(usize, usize)>>,
    bs: &mut Vec<Vec<i32>>,
    board_of_game: &mut Vec<Vec<i32>>,
) -> Result<(Vec<(usize, usize)>, Vec<(usize, usize)>), usize> {
    let block_num = bs.len();
    // let mut flag = false;
    let mut not_mine = vec![];
    let mut is_mine = vec![];
    let mut remove_blocks_id = vec![];
    for b in (0..block_num).rev() {
        let mut not_mine_rel = vec![];
        let mut is_mine_rel = vec![];
        let matrix_column = xs[b].len();
        let matrix_row = bs[b].len();
        if matrix_row <= 1 {
            continue; // 整段只有一个数字，比如角落的1
        }
        for i in 1..matrix_row {
            for j in 0..i {
                let mut ADval1 = vec![];
                let mut ADvaln1 = vec![];
                let mut FlagAdj = false;
                for k in 0..matrix_column {
                    if As[b][i][k] >= 1 && As[b][j][k] >= 1 {
                        FlagAdj = true;
                        continue;
                    }
                    if As[b][i][k] - As[b][j][k] == 1 {
                        ADval1.push(k)
                    } else if As[b][i][k] - As[b][j][k] == -1 {
                        ADvaln1.push(k)
                    }
                }
                if FlagAdj {
                    let bDval = bs[b][i] - bs[b][j];
                    if ADval1.len() as i32 == bDval {
                        is_mine_rel.append(&mut ADval1);
                        not_mine_rel.append(&mut ADvaln1);
                    } else if ADvaln1.len() as i32 == -bDval {
                        is_mine_rel.append(&mut ADvaln1);
                        not_mine_rel.append(&mut ADval1);
                    }
                }
            }
        }
        is_mine_rel.sort();
        is_mine_rel.dedup();
        not_mine_rel.sort();
        not_mine_rel.dedup();
        for i in &not_mine_rel {
            not_mine.push(xs[b][*i]);
            board_of_game[xs[b][*i].0][xs[b][*i].1] = 12;
        }
        for i in &is_mine_rel {
            is_mine.push(xs[b][*i]);
            board_of_game[xs[b][*i].0][xs[b][*i].1] = 11;
            for j in 0..As[b].len() {
                bs[b][j] -= As[b][j][*i];
            }
        }
        let mut del_id = not_mine_rel;
        del_id.append(&mut is_mine_rel);
        del_id.sort_by(|a, b| b.cmp(a));
        del_id.dedup();
        for i in del_id {
            xs[b].remove(i);
            for jj in 0..As[b].len() {
                As[b][jj].remove(i);
            }
        }
        if xs[b].is_empty() {
            remove_blocks_id.push(b);
        }
    }

    for b in remove_blocks_id {
        As.remove(b);
        bs.remove(b);
        xs.remove(b);
    }

    if SHOW_DEBUG > 999 {
        println!("As: {:?}\nxs: {:?}\nbs: {:?}", As, xs, bs);
        for ls in board_of_game.iter() {
            print!("{{");
            for v in ls.iter() {
                print!("{}, ", v);
            }
            print!("}},\n");
        }
        println!();
    }

    let (mut not, mut is) = solve_direct(As, xs, bs, board_of_game)?; // 没错，双集合判雷的最后一步是用单集合再过一轮。理由：（1）这样才不会报错（2）单集合复杂度很低，不费事
    not_mine.append(&mut not);
    is_mine.append(&mut is);
    chunk_matrixes(As, xs, bs);
    Ok((not_mine, is_mine))
}

pub fn combine(
    MatrixA: &Vec<Vec<i32>>,
    Matrixx: &Vec<(usize, usize)>,
) -> (Vec<Vec<i32>>, Vec<(usize, usize)>, Vec<Vec<usize>>) {
    // 检查地位完全相同的格子，全部返回。例如[[3,1,2],[0,5],[4],[6]]
    // MatrixA不能为空
    // 并在内部更改矩阵，合并重复的列
    let mut matrixA_squeeze = MatrixA.clone();
    let mut matrixx_squeeze = Matrixx.clone();
    let cells_num = matrixx_squeeze.len();
    let mut pair_cells = vec![];
    let mut del_cells = vec![]; // 由于重复需要最后被删除的列
    for i in 0..cells_num {
        pair_cells.push(vec![i]);
        for j in i + 1..cells_num {
            if !matrixA_squeeze.iter().any(|x| x[i] != x[j]) {
                pair_cells[i].push(j);
                del_cells.push(j);
            }
        }
    }
    del_cells.sort_by(|a, b| b.cmp(&a));
    del_cells.dedup();
    for i in del_cells {
        for r in 0..matrixA_squeeze.len() {
            matrixA_squeeze[r].remove(i);
        }
        matrixx_squeeze.remove(i);
        pair_cells.remove(i);
    }
    let cell_squeeze_num = pair_cells.len();
    for i in 0..cell_squeeze_num {
        let k = pair_cells[i].len() as i32;
        for r in 0..matrixA_squeeze.len() {
            matrixA_squeeze[r][i] *= k;
        }
    }
    (matrixA_squeeze, matrixx_squeeze, pair_cells)
}

fn cal_cell_and_equation_map(matrix_a: &Vec<Vec<i32>>) -> (Vec<Vec<usize>>, Vec<Vec<usize>>) {
    // cell_to_equation_map是方程中未知数的索引到方程的索引的映射
    // 方程中的未知数可以理解成未知的格子，每个方程可以理解成局面中的一个数字
    // 也可以理解成矩阵A的稀疏表示
    let cells_num = matrix_a[0].len();
    let equations_num = matrix_a.len();
    let mut cell_to_equation_map = vec![vec![]; cells_num];
    let mut equation_to_cell_map = vec![vec![]; equations_num];
    for i in 0..equations_num {
        for j in 0..cells_num {
            if matrix_a[i][j] >= 1 {
                equation_to_cell_map[i].push(j);
                cell_to_equation_map[j].push(i);
            }
        }
    }
    (cell_to_equation_map, equation_to_cell_map)
}

pub fn C_query<T, U>(n: T, k: U) -> usize
where
    T: Into<usize>,
    U: Into<usize>,
{
    // 查表计算8以内小数字的组合数
    let a = [
        [1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0],
        [1, 2, 1, 0, 0, 0, 0, 0, 0],
        [1, 3, 3, 1, 0, 0, 0, 0, 0],
        [1, 4, 6, 4, 1, 0, 0, 0, 0],
        [1, 5, 10, 10, 5, 1, 0, 0, 0],
        [1, 6, 15, 20, 15, 6, 1, 0, 0],
        [1, 7, 21, 35, 35, 21, 7, 1, 0],
        [1, 8, 28, 56, 70, 56, 28, 8, 1],
    ];
    a[n.into()][k.into()]
}

fn cal_table_minenum_recursion_step(
    idx: usize,
    current_amount: usize,
    table_minenum: &mut [Vec<usize>; 2],
    table_cell_minenum: &mut Vec<Vec<usize>>,
    // mut upper_limit: usize,
    // lower_limit: usize,
    matrixA_squeeze: &Vec<Vec<i32>>,
    matrix_b: &Vec<i32>,
    matrix_b_remain: &mut Vec<i32>,
    combination_relationship: &Vec<Vec<usize>>,
    cell_to_equation_map: &Vec<Vec<usize>>,
    equation_to_cell_map: &Vec<Vec<usize>>,
    mine_vec: &mut Vec<usize>,
) -> Result<bool, usize> {
    // mine_vec: 是雷位置都记录下来，只记录一个索引，可能有重复
    let cells_num = matrixA_squeeze[0].len();
    if idx >= cells_num {
        //终止条件
        let total_mines_num: usize = mine_vec.iter().sum();
        if total_mines_num >= table_minenum[1].len() {
            return Err(5);
        }
        table_minenum[1][total_mines_num] += current_amount;
        for (idn, n) in mine_vec.iter().enumerate() {
            table_cell_minenum[total_mines_num][idn] +=
                current_amount * n / combination_relationship[idn].len();
        }
        return Ok(true);
    }

    let mut upper_limit = combination_relationship[idx].len();
    let mut lower_limit = 0usize;
    for cell_i in &cell_to_equation_map[idx] {
        if matrixA_squeeze[*cell_i][idx] == 0 {
            continue;
        }
        let upper_limit_i = min(
            matrix_b_remain[*cell_i],
            combination_relationship[idx].len() as i32,
        );
        let mut lower_limit_i = matrix_b_remain[*cell_i];
        for j in &equation_to_cell_map[*cell_i] {
            if j > &idx {
                lower_limit_i -= combination_relationship[*j].len() as i32;
            }
        }
        if upper_limit_i < upper_limit as i32 {
            upper_limit = upper_limit_i as usize;
        }
        if lower_limit_i > lower_limit as i32 {
            lower_limit = lower_limit_i as usize;
        }
    }

    // println!("{:?}, {:?}", lower_limit, upper_limit + 1);
    // if lower_limit < upper_limit + 1 {
    for u in lower_limit..upper_limit + 1 {
        // let b = mine_vec[idx];
        mine_vec[idx] = u;
        if u > 0 {
            for tt in &cell_to_equation_map[idx] {
                matrix_b_remain[*tt] -= u as i32;
            }
        }
        let _ = cal_table_minenum_recursion_step(
            idx + 1,
            current_amount * C_query(combination_relationship[idx].len(), u),
            table_minenum,
            table_cell_minenum,
            &matrixA_squeeze,
            &matrix_b,
            matrix_b_remain,
            &combination_relationship,
            &cell_to_equation_map,
            &equation_to_cell_map,
            mine_vec,
        )?;
        for tt in &cell_to_equation_map[idx] {
            matrix_b_remain[*tt] += u as i32;
        }
        mine_vec[idx] = 0;
    }
    // }
    Ok(false)
}

pub fn cal_table_minenum_recursion(
    matrixA_squeeze: &Vec<Vec<i32>>,
    matrixx_squeeze: &Vec<(usize, usize)>,
    matrix_b: &Vec<i32>,
    combination_relationship: &Vec<Vec<usize>>,
) -> Result<([Vec<usize>; 2], Vec<Vec<usize>>), usize> {
    // 递归算法，得到雷数分布表和每格是雷情况数表，顺便计算最小、最大雷数
    // 输入矩阵必须是非空的，且行列数必须匹配
    // 行数和列数至少为1
    let cells_num = matrixx_squeeze.len();
    if cells_num > ENUM_LIMIT {
        // 超出枚举极限长度异常
        return Err(cells_num);
    }
    let cells_num_total = combination_relationship
        .iter()
        .fold(0, |item, x| item + x.len());
    // cells_num_total指合并前的格子数

    let mut flag_legal_board = true;
    let mut table_minenum: [Vec<usize>; 2] = [
        (0..cells_num_total + 1).collect::<Vec<usize>>(),
        vec![0; cells_num_total + 1],
    ];
    let (cell_to_equation_map, equation_to_cell_map) = cal_cell_and_equation_map(&matrixA_squeeze);
    // 计算两个映射表以减少复杂度
    // println!("cell_to_equation_map = {:?}; equation_to_cell_map = {:?}", cell_to_equation_map, equation_to_cell_map);

    let mut table_cell_minenum: Vec<Vec<usize>> = vec![vec![0; cells_num]; cells_num_total + 1];

    // println!("{:?}", matrixA_squeeze);
    cal_table_minenum_recursion_step(
        0,
        1,
        &mut table_minenum,
        &mut table_cell_minenum,
        &matrixA_squeeze,
        &matrix_b,
        &mut matrix_b.clone(),
        &combination_relationship,
        &cell_to_equation_map,
        &equation_to_cell_map,
        &mut (vec![0; cells_num]),
    )?;
    // println!("table_cell_minenum{:?}", table_cell_minenum);
    // println!("table_minenum{:?}", table_minenum);
    while table_minenum[1][0] == 0 {
        table_minenum[0].remove(0);
        table_minenum[1].remove(0);
        table_cell_minenum.remove(0);
        if table_cell_minenum.is_empty() {
            flag_legal_board = false;
            break;
        }
    }
    if flag_legal_board {
        while table_minenum[1][table_cell_minenum.len() - 1] == 0 {
            table_minenum[0].pop();
            table_minenum[1].pop();
            table_cell_minenum.pop();
        }
    }
    if flag_legal_board {
        Ok((table_minenum, table_cell_minenum))
    } else {
        return Err(1);
    }
}

/// 枚举法判雷引擎。
/// - 输入：分段好的矩阵、局面、枚举长度限制。
/// - 输出：是雷、不是雷的位置。
/// # Example
/// ```rust
/// let mut game_board = vec![
///     vec![0, 0, 1, 10, 10, 10, 10, 10],
///     vec![0, 0, 2, 10, 10, 10, 10, 10],
///     vec![1, 1, 3, 11, 10, 10, 10, 10],
///     vec![10, 10, 4, 10, 10, 10, 10, 10],
///     vec![10, 10, 10, 10, 10, 10, 10, 10],
///     vec![10, 10, 10, 10, 10, 10, 10, 10],
///     vec![10, 10, 10, 10, 10, 10, 10, 10],
///     vec![10, 10, 10, 10, 10, 10, 10, 10],
/// ];
/// let (As, xs, bs, _, _) = refresh_matrixs(&game_board);
/// let ans = solve_enumerate(&As, &xs, &bs, 40);
/// print!("{:?}", ans);
/// ```
/// - 注意：不修改输入进来的局面，即不帮助标雷（这个设计后续可能修改）；也不维护3个矩阵。因为枚举引擎是最后使用的
/// - 注意：超出枚举长度限制是未定义的行为，算法不一定会得到足够多的结果
pub fn solve_enumerate(
    As: &Vec<Vec<Vec<i32>>>,
    xs: &Vec<Vec<(usize, usize)>>,
    bs: &Vec<Vec<i32>>,
) -> (Vec<(usize, usize)>, Vec<(usize, usize)>) {
    if bs.is_empty() {
        return (vec![], vec![]);
    }

    if SHOW_DEBUG > 999 {
        println!("As:{:?}\nxs:{:?}\nbs:{:?}", As, xs, bs);
        println!();
    }

    let mut not_mine = vec![];
    let mut is_mine = vec![];
    let block_num = xs.len();

    let mut comb_relp_s = vec![];
    let mut matrixA_squeeze_s: Vec<Vec<Vec<i32>>> = vec![];
    let mut matrixx_squeeze_s: Vec<Vec<(usize, usize)>> = vec![];
    for i in 0..block_num {
        if xs[i].len() > ENUM_LIMIT {
            return (not_mine, is_mine);
        }
        let (matrixA_squeeze, matrixx_squeeze, combination_relationship) = combine(&As[i], &xs[i]);
        comb_relp_s.push(combination_relationship);
        matrixA_squeeze_s.push(matrixA_squeeze);
        matrixx_squeeze_s.push(matrixx_squeeze);
    }
    for i in 0..block_num {
        if SHOW_DEBUG > 999 {
            println!(
                "i: {:?}\nmatrixA_squeeze_s:{:?}\nmatrixx_squeeze_s:{:?}\nbs{:?}\ncomb_relp_s:{:?}",
                i, matrixA_squeeze_s[i], matrixx_squeeze_s[i], bs[i], comb_relp_s[i]
            );
            println!();
        }

        let (table_minenum_i, table_cell_minenum_i) = cal_table_minenum_recursion(
            &matrixA_squeeze_s[i],
            &matrixx_squeeze_s[i],
            &bs[i],
            &comb_relp_s[i],
        )
        .unwrap();

        if SHOW_DEBUG > 999 {
            println!(
                "table_minenum_i: {:?}\ntable_cell_minenum_i: {:?}",
                table_minenum_i, table_cell_minenum_i
            );
            println!();
        }

        for jj in 0..table_cell_minenum_i[0].len() {
            let mut s_num = 0; // 该合成格子的总情况数
            for ii in 0..table_cell_minenum_i.len() {
                s_num += table_cell_minenum_i[ii][jj];
            }
            if s_num == 0 {
                for kk in &comb_relp_s[i][jj] {
                    not_mine.push(xs[i][*kk]);
                }
            } else if s_num == table_minenum_i[1].iter().sum::<usize>() * comb_relp_s[i][jj].len() {
                for kk in &comb_relp_s[i][jj] {
                    is_mine.push(xs[i][*kk]);
                }
            }
        }
    }
    (not_mine, is_mine)
}

/// <span id="is_solvable">从指定位置开始扫，判断局面是否无猜。
/// - 注意：周围一圈都是雷，那么若中间是雷不算猜，若中间不是雷算有猜。
/// - 注意：不考虑剩余雷数。
pub fn is_solvable(board: &Vec<Vec<i32>>, touch_row: usize, touch_col: usize) -> bool {
    if unsolvable_structure(&board) {
        //若包含不可判雷结构，则不是无猜
        return false;
    }
    let row = board.len();
    let column = board[0].len();
    let mut board_of_game = vec![vec![10; column]; row];
    // 10是未打开，11是标雷
    // 局面大小必须超过6*6
    refresh_board(board, &mut board_of_game, vec![(touch_row, touch_col)]);

    if isVictory(&board_of_game, &board) {
        return true; // 暂且认为点一下就扫开也是可以的
    }

    loop {
        let (mut As, mut xs, mut bs, _, _) = refresh_matrixs(&board_of_game);

        if SHOW_DEBUG > 999 {
            println!("As: {:?}\nxs: {:?}\nbs: {:?}", As, xs, bs);
            println!();
        }

        let ans = solve_direct(&mut As, &mut xs, &mut bs, &mut board_of_game).unwrap();

        if SHOW_DEBUG > 999 {
            println!(
                "As: {:?}\nxs: {:?}\nbs: {:?}\nans.0: {:?}\nans.1: {:?}",
                As, xs, bs, ans.0, ans.1
            );
            println!();
        }

        let not_mine;
        if ans.0.is_empty() && ans.1.is_empty() {
            let ans = solve_minus(&mut As, &mut xs, &mut bs, &mut board_of_game).unwrap();
            if ans.0.is_empty() && ans.1.is_empty() {
                let ans = solve_enumerate(&As, &xs, &bs);
                if ans.0.is_empty() && ans.1.is_empty() {
                    return false;
                } else {
                    not_mine = ans.0
                }
                if !ans.1.is_empty() {
                    if SHOW_DEBUG > 999 {
                        println!("sure_mines: {:?}", ans.1);
                        println!();
                    }

                    for (o, p) in ans.1 {
                        board_of_game[o][p] = 11;
                    }
                }
            } else {
                not_mine = ans.0
            }
        } else {
            not_mine = ans.0
        }

        if SHOW_DEBUG > 999 {
            println!("not_mine: {:?}", not_mine);
            println!();
        }

        refresh_board(board, &mut board_of_game, not_mine);

        if isVictory(&board_of_game, &board) {
            return true;
        }
    }
}

/// 通用win7规则埋雷引擎。
/// - win7规则：起手位置开空，其余位置的雷服从均匀分布。
/// - 输出：二维的局面，其中0代表空，1~8代表1~8，-1代表雷。
pub fn laymine_op(
    row: usize,
    column: usize,
    minenum: usize,
    touchRow: usize,
    touchCol: usize,
) -> Vec<Vec<i32>> {
    let mut areaOp = 9;
    if touchRow == 0 || touchCol == 0 || touchRow == row - 1 || touchCol == column - 1 {
        if touchRow == 0 && touchCol == 0
            || touchRow == 0 && touchCol == column - 1
            || touchRow == row - 1 && touchCol == 0
            || touchRow == row - 1 && touchCol == column - 1
        {
            areaOp = 4;
        } else {
            areaOp = 6;
        }
    }
    let area = row * column - areaOp;
    let mut board1_dim = vec![0; area - minenum];
    board1_dim.append(&mut vec![-1; minenum]);

    unsafe {
        if g_seed < 0 {
            let mut rng = rand::thread_rng();
            board1_dim.shuffle(&mut rng);
        } else {
            g_seed = shuffle_vector(&mut board1_dim, g_seed);
        }
    }

    let mut Board = vec![vec![0; column]; row];
    let mut skip = 0;
    for i in 0..(area + areaOp) {
        let x = i % row;
        let y = i / row;
        if x <= touchRow + 1 && touchRow <= x + 1 && y <= touchCol + 1 && touchCol <= y + 1 {
            skip += 1;
            continue;
        }
        if board1_dim[i - skip] < 0 {
            Board[x][y] = -1;
            for j in max(1, x) - 1..min(row, x + 2) {
                for k in max(1, y) - 1..min(column, y + 2) {
                    if Board[j][k] >= 0 {
                        Board[j][k] += 1
                    }
                }
            }
        }
    }
    Board
}

/// 删选法单线程无猜埋雷。不可以生成任意雷密度的无猜局面。但雷满足均匀分布。
/// - 输入：高、宽、雷数、起手行数、起手列数、最大尝试次数。
/// - 返回：是否成功。
/// - 注意：若不成功返回最后生成的局面，此时则不一定无猜。
/// - 用python调用时的示例：
/// ```python
/// import ms_toollib as ms
/// board = laymine_solvable(16, 30, 99, 3, 20, 100000) # 在第3行、第20列开局
/// ```
pub fn laymine_solvable(
    row: usize,
    column: usize,
    minenum: usize,
    touch_row: usize,
    touch_col: usize,
    max_times: usize,
) -> (Vec<Vec<i32>>, bool) {
    let mut times = 0;
    let mut board;
    while times < max_times {
        board = laymine_op(row, column, minenum, touch_row, touch_col);
        times += 1;
        if is_solvable(&board, touch_row, touch_col) {
            println!("solved in {} times", times);
            return (board, true);
        }
    }
    board = laymine_op(row, column, minenum, touch_row, touch_col);
    (board, false)
}

fn main() {
    unsafe {
        g_seed = 123456789;
    }

    let row = 30;
    let column = 30;
    let minenum = 200;
    let touch_row = row / 2;
    let touch_col = column / 2;
    let max_times = 1000;
    let (mut board, flag) = laymine_solvable(row, column, minenum, touch_row, touch_col, max_times);
    if flag {
        println!("Success!");
    } else {
        println!("Failed!");
    }

    for ls in board.iter() {
        print!("[");
        for v in ls.iter() {
            print!("{:2}, ", v);
        }
        print!("],\n");
    }
}
