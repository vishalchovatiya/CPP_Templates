### How different object stores in memory ?

```
class Point
{
public:
	Point( float xval );
	virtual ~Point();
	
	float x() const;
	static int PointCount();

protected:
	virtual ostream& print( ostream &os ) const;

	float _x;
	static int _point_count;
};
```
- Simple object model of above class PIC
```
```
- Inheritence object model store PIC
