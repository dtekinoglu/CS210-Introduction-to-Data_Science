(define (problem puzzle)
(:domain grid-visit-all)
(:objects
        loc-x0-y0
        loc-x0-y1
        loc-x0-y2
        loc-x1-y0
        loc-x1-y1
        loc-x1-y2
        loc-x2-y0
        loc-x2-y1
        loc-x2-y2
- place
)

(:init
        (at-robot loc-x0-y0)
        (blocked loc-x1-y1)
        (blocked loc-x0-y1)
        (blocked loc-x2-y2)
        (blocked loc-x2-y1)
        (blocked loc-x0-y0)
        (connected loc-x0-y0 loc-x1-y0)
        (connected loc-x0-y0 loc-x0-y1)
        (connected loc-x0-y0 loc-x2-y0)
        (connected loc-x0-y0 loc-x0-y2)
        (connected loc-x0-y1 loc-x0-y0)
        (connected loc-x0-y1 loc-x0-y2)
        (connected loc-x0-y1 loc-x1-y1)
        (connected loc-x0-y1 loc-x2-y1)
        (connected loc-x0-y2 loc-x0-y0)
        (connected loc-x0-y2 loc-x0-y1)
        (connected loc-x0-y2 loc-x1-y2)
        (connected loc-x0-y1 loc-x2-y2)
        (connected loc-x1-y0 loc-x0-y0)
        (connected loc-x1-y0 loc-x2-y0)
        (connected loc-x1-y0 loc-x1-y1)
        (connected loc-x1-y0 loc-x1-y2)
        (connected loc-x1-y1 loc-x0-y1)
        (connected loc-x1-y1 loc-x2-y1)
        (connected loc-x1-y1 loc-x1-y0)
        (connected loc-x1-y1 loc-x1-y2)
        (connected loc-x1-y2 loc-x0-y2)
        (connected loc-x1-y2 loc-x2-y2)
        (connected loc-x1-y2 loc-x1-y1)
        (connected loc-x1-y2 loc-x1-y0)
        (connected loc-x2-y0 loc-x0-y0)
        (connected loc-x2-y0 loc-x1-y0)
        (connected loc-x2-y0 loc-x2-y1)
        (connected loc-x2-y0 loc-x2-y2)
        (connected loc-x2-y1 loc-x0-y1)
        (connected loc-x2-y1 loc-x1-y1)
        (connected loc-x2-y1 loc-x2-y0)
        (connected loc-x2-y1 loc-x2-y2)
        (connected loc-x2-y2 loc-x0-y2)
        (connected loc-x2-y2 loc-x1-y2)
        (connected loc-x2-y2 loc-x2-y1)
        (connected loc-x2-y2 loc-x2-y0)
)
(:goal
(and
        (blocked loc-x0-y0)
        (blocked loc-x0-y1)
        (blocked loc-x0-y2)
        (blocked loc-x1-y0)
        (blocked loc-x1-y1)
        (blocked loc-x1-y2)
        (blocked loc-x2-y0)
        (blocked loc-x2-y1)
        (blocked loc-x2-y2)
)
)
)