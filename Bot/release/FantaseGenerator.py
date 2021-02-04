import random
from pathlib import Path
import json
import time
from os import path

from rlbot.agents.base_loadout_generator import BaseLoadoutGenerator
from rlbot.matchconfig.loadout_config import LoadoutConfig


class SampleLoadoutGenerator(BaseLoadoutGenerator):
    def generate_loadout(self, player_index: int, team: int) -> LoadoutConfig:

        # You could start with a loadout based on a cfg file in the same directory as this generator
        loadout = self.load_cfg_file(Path('FantaseAppearance.cfg'), team)


        colour = random.randrange(0, 70)
        colour2 = random.randrange(0, 105)
        colour3 = random.randrange(0, 14)
        colour4 = random.randrange(0, 14)
        colour5 = random.randrange(0, 14)

        mod = colour % 10
        burnt_sienna = [2, 3, 4, 13, 14, 24, 42, 43, 44, 52, 53, 54, 62, 63, 64]
        purple = [9, 19]
        if team == 0:
            if 0 <= mod <= 2:
                paint = 7
            elif 3 <= mod <= 4:
                paint = 4
            else:
                if colour in purple:
                    paint = 8
                else:
                    paint = 5
        if team == 1:
            if 0 <= mod <= 1:
                paint = 13
            elif 2 <= mod <= 4:
                if colour in burnt_sienna:
                    paint = 6
                else:
                    paint = 10
            elif 5 <= mod <= 7:
                paint = 1
            else:
                paint = 9
        loadout.paint_config.goal_explosion_paint_id = paint
        loadout.paint_config.boost_paint_id = paint
        loadout.team_color_id = colour
        loadout.custom_color_id = colour2
        # loadout.paint_config.decal_paint_id = colour3
        # loadout.paint_config.car_paint_id  = colour3
        # loadout.paint_config.wheels_paint_id = colour3

        return loadout