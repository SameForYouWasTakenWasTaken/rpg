local e = Entity.create()
print("Entity", e, "is", e:IsValid())

e:Destroy()
print("Destroyed", e)
print("Entity", e, "is", e:IsValid())
