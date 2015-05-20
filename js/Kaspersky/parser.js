var Parser = function()
{
	var OpenNextFile = function()
	{
		...
		return {content: content,
			filename: filename,
			filetype: filetype}
	}

	var HaveNextFile = function() { ... }

	var processString = function(str) {...}

	var functions = {}

	functions["txt"] = function(content)
	{
		var x = 0;
		var length = content.length;
		var str = '';
		var arr = [];
		while (x < length)
		{
			symbol = content.charAt(x);
			if (symbol == '\n')
			{
				var li = document.createElement('li');
				li.innerHTML = str;
				str = '';
				processString(str);
				arr.push(li);
			}
			else
			{
				str += symbol;
			}
		}                         	
	}
 
	functions["xml"] = function(content) { ... }

	//Добавить новый формат можно вот так:
	functions["docx"] = function(content) { ... }


	var content = {};
 
	this.loadFiles = function()
	{
		var newContent = {};
		while (HaveNextFile())
		{
			file = OpenNextFile();
			content[file.filename] = parse(file.content, filetype);
			newContent[file.filename] = parse(file.content, filtype);
		}
		return newContent;                           	
	}
 
	var parse = function(content, filetype)
	{
		if (var func = functions[filetype.toLowerCase()])
		{
			func.Call ();
		}
		else
		{
			console.error('Unknown file type', filetype);
		}
	}
}

var body = document.getElementsById('content');
var parser = new Parser();
var totalLines = 0;
var maxLines = 500 * 10 / 12.5;

setInterval(function()
{
	var parser = new Parser();
	var res = parser.loadFiles();
	for (i = 0; i < 10; i++)
            {
		if (totalLines ++ <  maxLines)
			body.appendChild(res[i]);
            }
}, 5000);
