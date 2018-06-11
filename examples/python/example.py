from CommonDataRepresentation import cdr

d = cdr()
d.setInteger(1, 100)
d.setString(2, "hello")

val = d.getString(1)
if val:
    print("integer: {0}".format(val))

val = d.getString(2)
if val:
    print("string: {0}".format(val))

print("cdr: {0}".format(d.toString()))
