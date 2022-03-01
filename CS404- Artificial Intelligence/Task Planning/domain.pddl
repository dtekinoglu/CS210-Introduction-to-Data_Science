(define (domain grid-visit-all)
(:requirements :typing)
(:types        place - object)
(:predicates (connected ?x ?y - place)
             (at-robot ?x - place)
             (blocked ?x - place)
)

(:action move
:parameters (?curpos ?nextpos - place)
:precondition (and (at-robot ?curpos) (connected ?curpos ?nextpos) (not (blocked ?nextpos)))
:effect (and (at-robot ?nextpos) (not (at-robot ?curpos)) (blocked ?nextpos))
)

)


