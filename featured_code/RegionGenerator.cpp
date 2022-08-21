#include <iostream>

#include "vector"
/** Variables and constants **/

const int MaxSize = 8;
int HashKeys[MaxSize][MaxSize];
int random_state = 1804289387;

struct Region {
    int SquareCount;
    int Squares[MaxSize][2];
    int HashKey;
    int Corners[4] = {0};
};

std::vector<Region> AllRegions;
/** Declarations **/
int RandomInteger ();
void Expand(int);
int GetHash(Region*, int);
Region NewRegion (Region*, int, int);

int main() {
    // Initialize hash keys
    for (int a = 0; a < MaxSize; a++) {
        for (int b = 0; b < MaxSize; b++) {
            HashKeys[a][b] = RandomInteger();
        }
    }
    // Add S_1
    Region S_1;
    S_1.Squares[0][0] = 0;
    S_1.Squares[0][1] = 0;
    S_1.SquareCount = 1;
    S_1.HashKey = GetHash(&S_1, 0);
    AllRegions.push_back(S_1);

    // Grow the set of all regions.
    for(int n = 1; n < MaxSize; n++) {
        Expand(n);
        std::cout << AllRegions.size() << "\n";
    }

    return 0;
}

/** Functions **/

int RandomInteger () {
    int number = random_state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    random_state = number;
    return std::abs(number);
}


int GetHash(Region* region, int symmetry) {
    int s;
    int HashValue = 0;
    // Swap corners aptly
    int corners[4] = {region->Corners[0], region->Corners[1], region->Corners[2], region->Corners[3]};
    if (symmetry & 1) {
        s = corners[0];
        corners[0] = -corners[2];
        corners[2] = -s;
    }
    if (symmetry & 2) {
        s = corners[1];
        corners[1] = -corners[3];
        corners[3] = -s;
    }
    if (symmetry & 4) {
        corners[0] = corners[0] ^ corners[1];
        corners[1] = corners[1] ^ corners[0];
        corners[0] = corners[0] ^ corners[1];
        corners[2] = corners[2] ^ corners[3];
        corners[3] = corners[3] ^ corners[2];
        corners[2] = corners[2] ^ corners[3];
    }
    // compute hash
    int x, y;
    for (int i = 0; i < region->SquareCount; i++) {
        // Get transformed coordinates
        x = region->Squares[i][0];
        y = region->Squares[i][1];
        if (symmetry & 1) {
            x = -x;
        }
        if (symmetry & 2) {
            y = -y;
        }
        if (symmetry & 4) {
            x = x ^ y;
            y = y ^ x;
            x = x ^ y;
        }
        HashValue ^= HashKeys[x - corners[0]][y - corners[1]];
    }
    return HashValue;
}

void Expand (int n) {
    // Variables
    const int dx[] = {-1, -1, -1, 0, 1, 1,  1,  0};
    const int dy[] = {-1,  0,  1, 1, 1, 0, -1, -1};
    Region New;
    bool expand;
    bool add;
    int ComparisonHash;
    for (int i = 0; i < AllRegions.size(); i++) {
        // valid regions to expand
        if (AllRegions[i].SquareCount == n) {
            // loop over all squares of the region
            for (int j = 0; j < n; j++) {
                // find all neighboring squares which we can expand into
                for (int d = 0; d < 8; d++) {
                    expand = true;
                    for (int k = 0; k < n; k++) {
                        if (AllRegions[i].Squares[j][0] + dx[d] == AllRegions[i].Squares[k][0] &&
                            AllRegions[i].Squares[j][1] + dy[d] == AllRegions[i].Squares[k][1]) {
                            expand = false;
                            break;
                        }
                    }
                    // check isomorphism
                    if (expand) {
                        add = true;
                        New = NewRegion(&AllRegions[i], AllRegions[i].Squares[j][0] + dx[d], AllRegions[i].Squares[j][1] + dy[d]);
                        // Check if hashes are unique
                        for (int s = 0; s < 8; s++) {
                            ComparisonHash = GetHash(&New, s);
                            for (int i = AllRegions.size(); i >= 0; i--) {
                                if (AllRegions[i].SquareCount == n) {
                                    break;
                                }
                                if (AllRegions[i].HashKey == ComparisonHash) {
                                    add = false;
                                    break;
                                }
                            }
                            if (!add) {
                                break;
                            }
                        }
                        // Add region to total set
                        if (add) {
                            AllRegions.push_back(New);
                        }
                    }
                }
            }
        }
    }
    return;
}

Region NewRegion (Region* parent, int x, int y) {
    Region Child;
    // Copy parent
    for (int i = 0; i < parent->SquareCount; i++) {
        Child.Squares[i][0] = parent->Squares[i][0];
        Child.Squares[i][1] = parent->Squares[i][1];
    }
    // Add new square
    Child.Squares[parent->SquareCount][0] = x;
    Child.Squares[parent->SquareCount][1] = y;
    // Update characteristics
    Child.SquareCount = parent->SquareCount + 1;
    Child.Corners[0] = std::min(x, parent->Corners[0]);
    Child.Corners[1] = std::min(y, parent->Corners[1]);
    Child.Corners[2] = std::max(x, parent->Corners[2]);
    Child.Corners[3] = std::max(y, parent->Corners[3]);
    Child.HashKey = GetHash(&Child, 0);
    return Child;
}
