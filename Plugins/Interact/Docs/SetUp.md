Needed to set up

# Set up collision:
go to project settings > collision


## add object channel
you need one for the hints and one for the interacts. (the hint is optional)
Name: Hint
DefaultResponse: Ignore

Name: Interact
DefaultResponse: Ignore

## add a trace channel
this is used to detect the interactions with the CInteractor
Name: InterTrace
DefaultResponse: Ignore

## add profiles
you need one for the interact and one for the hint object (optional)

Name : Interact
Collision: Query Only
Object Type: Interact
Description: Interact object

Set everything to ignore. except:
Trace:
* Visibility
* Camera
* Interact
set them to block

ObjectType:
* Hint: set to overlap



Name : Hint
Collision: Query Only
Object Type: Hint
Description: Hint object

Set everything to ignore. except:
ObjectType:
* Interact: set to overlap




// You also need to set up a post process volume with a material set for the hover effect. for example MI_PostHover
// hint: you might want to make it unbound

by default all interact will have the collision profile "Interact"
you can set it by calling
UCInteract::SetCollisionProfile

