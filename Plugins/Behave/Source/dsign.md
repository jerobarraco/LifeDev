| Dimensions 	| Has		| Does		| Wants		| Needs 	|
------------------------------------------
Space           | Position	 Move		 Go To
				  Rotation	 Open?		 Get Item
				  Scale


Emotional		 Anger/Fear/etc

Biological		 Energy/Hunger/Temp

Time			 ???

* Add UBParallel.
* * decide what to do when one finishesaoeu
* 

See this one
https://www.youtube.com/watch?v=gm7K68663rA#
it's loosely based on the first person.
And the comment about Layered GOAP

# TODO challenges
* A* plan search
* Random Pick UBBase
* Randomize costs


# note
currently i have the actions do the actual logic and controlling the pawn.
a more general way would be to have the pawn listen to the current action and act accordingly. stopping the action if necessary. but i think that adds complexity and can make things more difficult.
you can do it that way if you prefer.

# TODO

* preferences/traits
* modifies the values of certain wants.
* could also modify values of the "damps" (at least on Do)
* traits could be 
* * personal
* * environmental (added/removed depending on the place)
* * some behaves can add/remove traits (e.g. sadness adds "slow" and "tiresome")
* * traits can also be queried to inform the representation (visual/audio/etc)

* actually have the components do stuff
* * move an actor, change an animation, set a color, display a dialog.

* have a way to influence the cbehavior from the evironment. or maybe other components.
* * e.g. via events that trigger a token.

* component that chooses goal based on token
* * maybe have tree

want
need
react : maps an effect to an outcome

Have concurrent actions.
* Maybe have a map with the current Behave and want. When a behave Do returns Doing

Once the plan is empty. wait a bit before choosing a new want
// TODO wait and let want arise normally. have a period of satisfaction.

react: can be optimized by being called less often


might wanna check this talk and refactor how it works
https://www.youtube.com/watch?v=IvK0ZlNoxjw

specially the 2nd speaker.
* divide decision and actions
* have curve modifiers
* have veto
* 