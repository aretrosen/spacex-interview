#include "util.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

std::map<User, std::pair<Sat, Color>>
   solve(const std::map<User, Vector3> &users,
         const std::map<Sat, Vector3> &sats) {
   std::map<User, std::pair<Sat, Color>> solution;

   const Vector3 center = Vector3{0, 0, 0};
   std::unordered_map<Sat, std::vector<std::pair<float, User>>> possible_users(
      sats.size());

   for (const auto &[sat_id, sat_loc] : sats) {
      for (const auto &[user_id, user_loc] : users) {
         auto angle = 180 - std::abs(user_loc.angle_between(center, sat_loc));
         if (angle <= 45.0) {
            possible_users[sat_id].push_back({angle, user_id});
         }
      }
   }

   std::unordered_map<User, bool> user_done(users.size());

   for (auto &[sat_id, pos_users] : possible_users) {
      int num = pos_users.size();
      std::sort(pos_users.begin(), pos_users.end());
      std::vector<User> valid_users;
      valid_users.reserve(num);
      for (int i = 0; i < num; ++i) {
         if (user_done[pos_users[i].second]) continue;
         valid_users.push_back(pos_users[i].second);
      }
      num = valid_users.size();

      int l_idx[4] = {-1, -1, -1, -1};
      int c_idx = 0;
      int cnt_sats = 0;

      for (int idx = 0; idx < num && cnt_sats < 32; ++idx) {
         if (l_idx[c_idx] != -1 &&
             std::abs(sats.at(sat_id).angle_between(
                users.at(valid_users[idx]), users.at(l_idx[c_idx]))) < 10.0)
            continue;

         if (l_idx[c_idx] != -1) {
            std::cout << std::abs(sats.at(sat_id).angle_between(
                            users.at(valid_users[idx]), users.at(l_idx[c_idx])))
                      << ' ' << valid_users[idx] << ' ' << l_idx[c_idx] << '\n';
         }

         ++cnt_sats;
         l_idx[c_idx] = valid_users[idx];
         solution[valid_users[idx]] = {sat_id, colors[c_idx++]};
         user_done[valid_users[idx]] = true;
         c_idx %= 4;
      }
   }

   return solution;
}
