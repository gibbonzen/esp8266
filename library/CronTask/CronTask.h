import { Clock } from "../clock/Clock";
import { ClockObserver } from "../Clock/ClockObserver";
import { TimerUtils } from "../TimerUtils/TimerUtils";

export class CronTask {
    private hour: number
    private min: number
    private sec: number
    private time: number
    private listener: Function

    private observer: ClockObserver

    constructor(time: string, listener: Function) {
        let timeUnits = time.split(":")
        this.hour = Number(timeUnits[0]) | 0
        this.min = Number(timeUnits[1]) | 0
        this.sec = Number(timeUnits[2]) | 0
        this.time = TimerUtils.convert(this.hour, "h", "s") + TimerUtils.convert(this.min, "m", "s") + this.sec
        this.listener = listener
    }

    attachTimer(clock: Clock) {
        this.observer = new ClockObserver(clock)
        this.observer.onTime(this.time, this.listener)
    }

    start() {
        this.observer.start()
    }

    stop() {
        this.observer.stop()
        this.observer = null
    }

}