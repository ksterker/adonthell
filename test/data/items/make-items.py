import adonthell
import rechargable_lightsource

# -- create mutable items
item = adonthell.item_base (1)
item.set_max_stack (5)
item.create_instance ("lightsource", "lightsource")
pyitem = item.get_instance ()


# -- small torch
print "Small Torch ..."
pyitem.Charge = 10
pyitem.Duration = "5m"
pyitem.Name = "Small Torch"
pyitem.Value = 5

item.save ("small_torch.item")


# -- large torch
print "Large Torch ..."
pyitem.Charge = 15
pyitem.Duration = "8m"
pyitem.Name = "Large Torch"
pyitem.Value = 8

item.save ("large_torch.item")


# -- candle
print "Candle ..."
pyitem.Charge = 20
pyitem.Duration = "10m"
pyitem.Name = "Candle"
pyitem.Value = 10

item.set_max_stack (10)
item.save ("candle.item")


# -- lamp
print "Oil Lamp ..."
pyitem = None
item.create_instance ("rechargable_lightsource", "rechargable_lightsource")
item.set_max_stack (1)
 
pyitem = item.get_instance ()
pyitem.Charge = 6
pyitem.MaxCharge = 12
pyitem.Duration = "5m"
pyitem.Name = "Oil Lamp"
pyitem.Value = 15

item.save ("lamp.item")


# -- lamp oil
print "Oil Flask ..."
pyitem = None
item.create_instance ("item", "item")
item.set_max_stack (5)

pyitem = item.get_instance ()
pyitem.Charge = 24
pyitem.MaxCharge = 24
pyitem.Name = "Oil Flask"
pyitem.Value = 6
pyitem.add_category ("Lampfuel")

item.save ("oil_flask.item")


# -- poison
print "Nightshade Syrup ..."

pyitem.Charge = 30
pyitem.MaxCharge = 30
pyitem.Name = "Nightshade Syrup"
pyitem.Value = 80
pyitem.add_category ("Poison")

item.save ("nightshade_syrup.item")


# -- create immutable items

# -- coin
print "Coin ..."
pyitem = None
item = adonthell.item_base (0)
item.set_max_stack (1000)
item.create_instance ("item", "item")

pyitem = item.get_instance ()
pyitem.Name = "Coin"
pyitem.Value = 0

item.save ("coin.item")


# -- knife
print "Knife ..."
item.set_max_stack (1)

pyitem.Name = "Knife"
pyitem.Value = 4

item.save ("knife.item")


# -- stick
print "Stick ..."
pyitem = None
item.set_max_stack (40)
item.create_instance ("transformable_item", "transformable_item")
pyitem = item.get_instance ()

pyitem.Name = "Stick"
pyitem.Agent = "Knife"
pyitem.Result = "Arrow"
pyitem.Value = 0

item.save ("stick.item")


# -- arrow
print "Arrow ..."
pyitem = None
item.set_max_stack (50)
item.create_instance ("projectile", "projectile")
pyitem = item.get_instance ()

pyitem.Name = "Arrow"
pyitem.Value = 1

item.save ("arrow.item")


