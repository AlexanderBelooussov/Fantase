//
// Created by Alex on 06/08/2020.
//

#include "Shot.h"
#include "../../utils/misc.h"
#include "../../utils/vector_math.h"


Shot::Shot(GameInfo &info) : Maneuver(info), move(info), targeting(info) {
}

bool shotPredictionCondition(Car &car, Ball &ball) {
    return true;
}

void Shot::updatePrediction(const std::function<bool(Car&, Ball&)>& cond) {
    prediction = Prediction(info.myCar, info.ball, cond);

    if (allow_backwards){
        Prediction backwards_prediction = Prediction(info.myCar, info.ball, cond, true);
        if (backwards_prediction.time + 0.1 < prediction.time){
            prediction = backwards_prediction;
            backwards = true;
        }
    }

    //self.configure(self.intercept)
    //self._last_update_time = self.car.time
    if (!prediction.is_viable) // or prediction.time > self._initial_time + self.max_additional_time:)
        finished = true;
}

void Shot::step() {
    std::function<bool(Car&, Ball&)> cond = shotPredictionCondition;
    updatePrediction(cond);
}

void Shot::initialPrediction() {
    std::function<bool(Car&, Ball&)> cond = shotPredictionCondition;
    updatePrediction(cond);
}
