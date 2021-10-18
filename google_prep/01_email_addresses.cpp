#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::pair<std::string, std::string> splitLocalAndDomain(
    std::string const& email) {
  auto i = email.find('@');
  return {
      std::string(email.begin(), email.begin() + i),
      std::string(email.begin() + i + 1, email.end()),
  };
}

std::string cleanLocalName(std::string const& localName) {
  std::size_t n = localName.find_first_of('+');

  std::string working(localName);

  if (n != std::string::npos) {
    working = std::string(localName.begin(), localName.begin() + n);
  }

  auto noDots = std::remove(working.begin(), working.end(), '.');
  working.erase(noDots, working.end());

  return working;
}

class Solution {
 public:
  int numUniqueEmails(std::vector<std::string> const& emails) {
    std::unordered_map<std::string, std::vector<std::string>> data;

    unsigned int total = 0;
    for (std::string const& email : emails) {
      auto const [localName, domain] = splitLocalAndDomain(email);
      auto const cleanLocal = cleanLocalName(localName);

      auto it = data.find(domain);

      if (it == data.end()) {
        data.emplace(make_pair(domain, std::vector<std::string>{cleanLocal}));
        total += 1;
        continue;
      }

      std::vector<std::string>& vals = it->second;
      if (std::find(vals.begin(), vals.end(), cleanLocal) == vals.end()) {
        total += 1;
        vals.push_back(cleanLocal);
      }
    }

    return total;
  }
};

int main() {
  Solution sol;

  int n = sol.numUniqueEmails({"test.email+alex@leetcode.com",
                               "test.e.mail+bob.cathy@leetcode.com",
                               "testemail+david@lee.tcode.com"});

  std::cout << n << " unique emails" << '\n';
}
