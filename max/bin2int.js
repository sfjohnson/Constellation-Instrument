inlets = 1;
outlets = 2;

function anything()
{
	var place = 1;
	var total = 0;
	for (var i = arguments.length-1; i >= 0; i--)
	{
		if (arguments[i] > 0)
			total += place;
		
		place <<= 1;
	}
	
	outlet(1, place);
	outlet(0, total);
}