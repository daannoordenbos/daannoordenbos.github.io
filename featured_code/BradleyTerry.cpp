#include <iostream>
#include <fstream>

#include <string>
#include "vector"
#include <cmath>
#include <algorithm>


double lambda_lr(std::vector<std::vector<double>>& scores, std::vector<double>& p, double x, int k) {
    double result = 0;
    for (int j = 0; j < p.size(); j++) {
        if (j == k)
            continue;
        result += -x * scores[k][j] + (scores[k][j] + scores[j][k]) * (log(exp(p[k] + x) + exp(p[j])) - log(exp(p[k]) + exp(p[j])));
    }
    return result;
}

double del_lambda_lr(std::vector<std::vector<double>>& scores, std::vector<double>& p, double x, int k) {
    double result = 0;
    for (int j = 0; j < p.size(); j++) {
        if (j == k)
            continue;
        result += -scores[k][j] + (scores[k][j] + scores[j][k]) * exp(p[k] + x) / (exp(p[k] + x) + exp(p[j]));
    }
    return result;
}

struct Player {
    std::string name;
    double rating;
    double lb;
    double ub;
};

int main()
{
    // Get the raw data from the csv
    char delimiter = ';';
    std::vector<std::string> raw_data;
    std::ifstream data;
    std::string line;
    data.open("Test Table 3.csv");
    while (!data.eof()) {
        getline(data, line);
        raw_data.push_back(line + delimiter);
    }

    // Reformatting into a player list and a cross table
    int amount = raw_data.size() - 2;
    int longest_name = 0;
    std::vector<Player> player(amount);
    std::vector<std::vector<double>> cross(amount, std::vector<double>(amount));
    int k, l;

    for (int i = 1; i < amount + 1; i++) {
        k = -2;
        l = 0;
        for (int j = 0; j < raw_data[i].size(); j++) {
            if (raw_data[i][j] == delimiter) {
                if (k == -1) {
                    player[i - 1].name = raw_data[i].substr(l, j - l);
                    longest_name = std::max(longest_name, (int) player[i - 1].name.size());
                } else if (k > 0){
                    if (j - l == 0 || raw_data[i][j - 1] == '-') {
                        cross[i - 1][k - 1] = 0.0;
                    } else {
                        cross[i - 1][k - 1] = std::stof(raw_data[i].substr(l, j - l));
                    }
                }
                k++;
                l = j + 1;
            }
        }
    }
    // Computing the MLE estimates
    std::vector<double> beta(amount, 1.0);
    std::vector<double> w(amount, 0);
    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < amount; j++) {
            w[i] += cross[i][j];
        }
    }
    double div;
    double sum;
    for (int t = 0; t < 150; t++) {
        for (int k = 0; k < amount; k++) {
            div = 0;
            for (int j = 0; j < amount; j++) {
                div += (cross[k][j] + cross[j][k]) / (beta[j] + beta[k]);
            }
            beta[k] = w[k] / div;
            sum += beta[k];
        }

        for (int k = 0; k < amount; k++) {
            beta[k] = beta[k] / sum;
        }

    }
    // Transforming the MLE estimates
    double S = 400 / log(10);
    double lowest = 0;
    for (int k = 0; k < amount; k++) {
       beta[k] = log(beta[k]);
       lowest = std::min(lowest, beta[k]);

    }
    for (int k = 0; k < amount; k++) {
       beta[k] = beta[k] - lowest;
       player[k].rating = S * beta[k];
    }

    // Computing confidence intervals
    double dx;
    double x;
    for (int k = 0; k < amount; k++) {
        x = 0.1;
        for (int i = 0; i < 20; i++) {
            dx = (lambda_lr(cross, beta, x, k) - 2) / del_lambda_lr(cross, beta, x, k);
            x -= dx;
            if (std::abs(dx) < 0.0001)
                break;
        }
        player[k].ub = S * x;

        x = -0.1;
        for (int i = 0; i < 20; i++) {
            dx = (lambda_lr(cross, beta, x, k) - 2) / del_lambda_lr(cross, beta, x, k);
            x -= dx;
            if (std::abs(dx) < 0.0001)
                break;
        }
        player[k].lb = S * x;
    }
    // Sort on rating
    std::sort(player.begin(), player.end(), [](Player a, Player b) {
		return a.rating > b.rating;
	});

    std::cout << "Rank";
    for (int s = 0; s < 5 + longest_name; s++) std::cout << " ";
    std::cout << "Elo   +   -\n";
    for (int k = 0; k < amount; k++) {
        if (k < 9) std::cout << " ";
        std::cout << "  " << k + 1 << ": " << player[k].name;
        for (int s = 0; s < 5 + longest_name - player[k].name.size() - log10(player[k].rating + 1); s++) std::cout << " ";
        std::cout << (int) player[k].rating << "  +" << (int) player[k].ub << " " << (int) player[k].lb << "\n";
    }
    return 0;
}




