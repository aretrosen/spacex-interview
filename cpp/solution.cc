#include "util.h"

#include <algorithm>
#include <unordered_map>

const double user_sat_angle = 45.0;
const double sat_sat_angle = 10.0;
const int beam_per_color = 8;

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
         if (angle <= user_sat_angle) {
            possible_users[sat_id].push_back({angle, user_id});
         }
      }
   }

   std::unordered_map<User, bool> user_done(users.size());

   for (auto &[sat_id, pos_users] : possible_users) {
      int num = pos_users.size();
      std::sort(pos_users.begin(), pos_users.end());

      for (char color : colors) {
         std::vector<User> uuids;
         for (int i = 0; i < num && uuids.size() < beam_per_color; ++i) {
            User uuid = pos_users[i].second;
            if (user_done[uuid]) continue;
            bool break_condition = true;
            for (auto old_id : uuids) {
               if (std::abs(sats.at(sat_id).angle_between(
                      users.at(old_id), users.at(uuid))) < sat_sat_angle) {
                  break_condition = false;
                  break;
               }
            }
            if (!break_condition) continue;
            uuids.push_back(uuid);
            user_done[uuid] = true;
            solution[uuid] = {sat_id, color};
         }
      }
   }

   return solution;
}
