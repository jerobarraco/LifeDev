| Dimensions 	| Has		| Does		| Wants		| Needs 	|
------------------------------------------
Space           | Position	 Move		 Go To
				  Rotation	 Open?		 Get Item
				  Scale

Time			 ???

Emotional		 Anger/Fear/etc

Biological		 Energy


TODO

* component
* behavior

* component with preferences (and personality) which states which things triggers which effects
* * preferences could be on any dimension
* component that chooses goal based on token
* * maybe have tree

want
need
react : maps an effect to an outcome

Have concurrent actions.
* Maybe have a map with the current Behave and want. When a behave Do returns Doing

Once the plan is empty. wait a bit before choosing a new want
// TODO wait and let want arise normally. have a period of satisfaction.

need a way to override lower prio wants while they are working

react: can be optimized by being called less often

want: store how much i want it. for new checks.

way to interrupt actions, 

todo remove "Want" variable just use plan

todo refactor so that an action triggers an effect and the effect is applied.
e.g calling Do(Move) will trigger the event Tired*3 calling Do(Play) triggers Tired*2
think about it actually.