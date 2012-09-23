inlets = 1;
outlets = 2;

var table = new Array();
var tableLength = 0;
var baseSeed = (new Date()).getTime();
baseSeed %= 10000;

//http://michalbe.blogspot.com.au/2011/02/javascript-random-numbers-with-custom_23.html
/*function CustomRandom(nseed)
{  
	var seed = nseed;     
    var constant = Math.pow(2, 13)+1;
    var prime = 1987;
    var maximum = 1000; //maximum number needed for calculation the float precision of the numbers (10^n where n is number of digits after dot)

    return
	{
		nextRand: function(lowest, highest)
		{
			post(this.seed);
			
			while (seed > constant)
				seed /= prime; 
			
        	seed *= constant; 
        	seed += prime;

        	return lowest && highest ? lowest+seed%maximum/maximum*(highest-lowest) : seed%maximum/maximum; // if 'min' and 'max' are not provided, return random number between 0 & 1    
		}
	};
}*/

function CustomRandom(nseed)
{
	this.seed = nseed;     
    this.constant = Math.pow(2, 13)+1;
    this.prime = 1987;
    this.maximum = 1000; //maximum number needed for calculation the float precision of the numbers (10^n where n is number of digits after dot)
}

CustomRandom.prototype.nextRand = function(lowest, highest)
{
	while (this.seed > this.constant)
		this.seed /= this.prime; 
			
	this.seed *= this.constant; 
	this.seed += this.prime;

	return lowest && highest ? lowest+this.seed%this.maximum/this.maximum*(highest-lowest) : this.seed%this.maximum/this.maximum; // if 'min' and 'max' are not provided, return random number between 0 & 1    
}

/*function anything()
{
	if (arguments.length == 4 && arguments[0] == "fill") //fill size min max
	{
		length = arguments[1];
		var rng = CustomRandom(baseSeed + length);
		for (var i = 0; i < length; i++)
			table[i] = nextRand(arguments[2], arguments[3]);
			
		outlet(0, length);
	}
	else if (arguments.length == 1 && arguments[0] < length)
		outlet(0, table[arguments[0]]);
}*/

function fill()
{
	if (arguments.length == 3) //size min max
	{
		tableLength = Number(arguments[0]);
		var rng = new CustomRandom(baseSeed + tableLength);
		for (var i = 0; i < tableLength; i++)
			table[i] = rng.nextRand(arguments[1], arguments[2]);

		outlet(1, tableLength);
	}
}

function msg_int(v)
{
	if (v < tableLength && v >= 0)
		outlet(0, table[v]);
}