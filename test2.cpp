#include<iostream>
#include<vector>
int maxnull(std::vector<int> rowdata) {
    int maxlen = 0;
    int left = 0;
    int right = 0;
    for (int i = 0; i < rowdata.size(); ++i) {
        int currentlen = right - left;
        if (rowdata[i] == 0) {
            right = i;
        }
        if (rowdata[i] == 1) {
            currentlen = right - left;
        }
        maxlen = std::max(currentlen, maxlen);
    }
    return maxlen;
}
int main() {
    int alldataNum;
    std::cin >> alldataNum;
    int datasize;
    int maxnull;
    std::vector<int> result;
    std::vector<std::vector<int>> datanum;
    for (int i = 0; i < alldataNum; ++i) {
        while (std::cin >> datasize) {
            for (int j = 0; j < datasize; ++j) {
                int data;
                std::cin >> data;
                datanum[j].push_back(data);
            }
        }
    }
    for (const auto& rownum : datanum) {
        int maxrow = maxnull(rownum);
        for (const auto& colnum : rownum) {
            if (colnum == 1) {
                colnum == 0;
                int newmaxrow = maxnull(rownum);
                if (newmaxrow > maxrow) {
                    maxrow = newmaxrow;
                }
            }
        }
        result.push_back(maxrow);
    }
    for (auto& rel : result) {
        std::cout << rel << std::endl;
    }
    return 0;
}