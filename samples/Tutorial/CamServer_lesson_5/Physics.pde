import pbox2d.*;
import org.jbox2d.collision.shapes.*;
import org.jbox2d.common.*;
import org.jbox2d.dynamics.*;

PBox2D box2d;
// A list for all of our rectangles
ArrayList boxList;

void physics_setup()
{
  // Initialize box2d physics and create the world
  box2d = new PBox2D(this);
  box2d.createWorld();
  // We are setting a custom gravity
  box2d.setGravity(0, -20); 

  boxList = new ArrayList();
}

void physics_draw()
{
  box2d.step();

  //add box randomly
  if (random(1) > 0.8)
  {
    Box p = new Box(width/2,random(30,70));
    boxList.add(p);
  }

  // Display all the boxes
  for (int i = 0; i < boxList.size(); i++) {
    Box p = (Box) boxList.get(i);
    p.display();
  }

  // boxes that leave the screen, we delete them
  // (note they have to be deleted from both the box2d world and our list
  for (int i = boxList.size()-1; i >= 0; i--) {
    Box p = (Box) boxList.get(i);
    if (p.done()) {
      boxList.remove(i);
    }
  }
}

void physics_clear()
{//clear box
  for (int i = boxList.size()-1; i >= 0; i--) 
  {
    Box p = (Box) boxList.get(i);
    p.killBody();
    boxList.remove(i);
  }
}

