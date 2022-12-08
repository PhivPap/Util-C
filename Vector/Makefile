demos:
	$(CC) -Wall -g -o demo Vector.c Demo.c
	$(CC) -Wall -g -o demo2 Vector.c Demo2.c
	$(CC) -Wall -g -o demo_serialization Vector.c DemoSerialization.c ../Serialization/serializers.c
	$(CC) -Wall -g -o demo_deserialization Vector.c DemoDeserialization.c ../Serialization/deserializers.c

clean:
	rm -f demo demo2 demo_serialization demo_deserialization