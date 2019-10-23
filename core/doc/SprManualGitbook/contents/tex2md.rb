files = [
	# "getstarted","structure", "base",
	"foundation","collision","physics","graphics","fileio","humaninterface","creature","framework","embpython","unity","trouble"
]

cnt = 3
for file in files

	ifp = open("../../SprManual/#{file}.tex")
	ofp = open("#{cnt}_#{file}.md", "w")

	lines = ""

	block_level = 0
	block_type = ""

	ignore = false

	table_columns = 1

	line_count_in_tabular = 0

	while line = ifp.gets

		line.encode!("UTF-8")

		enter_block = false
		exit_block = false
		independent_line = false

		line.rstrip! if block_level == 0

		line.gsub!(/\\path\{([^\}]+)\}/){ |match| "`#{$1}`" }
		line.gsub!(/\\url\{([^\}]+)\}/){ |match| "`#{$1}`" }
		line.gsub!(/\\noindent/){ |match| "\n" }
		line.gsub!(/\$([a-zA-Z0-9\-_]+)\$/){ |match| "*#{$1}*" }

		line.gsub!(/\\textbf\{([^\}]+)\}/){ |match| "**#{$1}**" }
		line.gsub!(/\\texttt\{([^\}]+)\}/){ |match| "*#{$1}*" }
		line.gsub!(/\\text\{([^\}]+)\}/){ |match| "#{$1}" }

		line.gsub!(/\\label\{([^\}]+)\}/){ |match| "" }
		line.gsub!(/\\index\{([^\}]+)\}/){ |match| "" }
		line.gsub!(/^\\newcommand.*$/){ |match| "" }

		line.gsub!(/\\begin\{align\*?\}/){ |match| enter_block = true; block_type = "align"; "" }
		line.gsub!(/\\end\{align\*?\}/){ |match| exit_block = true; "" }

		line.gsub!(/\\begin\{center\}/){ |match| "" }
		line.gsub!(/\\end\{center\}/){ |match| "" }

		line.gsub!(/\\begin\{comment\}/){ |match| "" }
		line.gsub!(/\\end\{comment\}/){ |match| "" }

		line.gsub!(/\\begin\{chapterabstract\}/){ |match| "" }
		line.gsub!(/\\end\{chapterabstract\}/){ |match| "" }

		line.gsub!(/\\begin\{sourcecode\*?\}/){ |match| enter_block = true; 
			block_type = "sourcecode"
			if file=="embpython"
				"```python"
			elsif file=="unity"
				"```c#"
			else
				"```c++"
			end
		}
		line.gsub!(/\\end\{sourcecode\*?\}/){ |match| exit_block = true; "```" }

		line.gsub!(/\\begin\{verbatim\*?\}/){ |match| enter_block = true; 
			block_type = "sourcecode"
			if file=="embpython"
				"```python"
			elsif file=="unity"
				"```c#"
			else
				"```c++"
			end
		}
		line.gsub!(/\\end\{verbatim\*?\}/){ |match| exit_block = true; "```" }


		line.gsub!(/\\begin\{figure.*$/){ |match| enter_block = true; ignore = true; block_type = "figure"; "" }
		line.gsub!(/\\end\{figure.*$/){ |match| exit_block = true; ignore = false; "" }

		line.gsub!(/\\begin\{table.*$/){ |match| enter_block = true; ignore = true; block_type = "table"; "" }
		line.gsub!(/\\end\{table.*$/){ |match| exit_block = true; ignore = false; "" }

		line.gsub!(/\\begin\{tabular.*$/){ |match| enter_block = true; block_type = "tabular"; line_count_in_tabular = 0; "" }
		line.gsub!(/\\end\{tabular.*$/){ |match| exit_block = true; "" }
		if (block_level > 0 and block_type == "tabular") 
			line.rstrip!

			line.gsub!(/\\multicolumn\{([1-9])\}\{.\}\{(.+)\}/) { |match| "#{$2}" + (" & " * ($1.to_i - 1)) }

			table_columns = line.count("&") + 1

			header_separater = "|" + (0...table_columns).to_a.map{"---"}.join("|") + "|\n"
			midrule_found = false

			line.gsub!("\\midrule") { |match| midrule_found = true; "" }

			line.gsub!(/\\/, "")

			line.gsub!("|", "\\|")

			line.gsub!(/[ \t　]&/, "|")

			line = "|#{line}|\n" if line != ""

			if line_count_in_tabular == 1
				if not midrule_found
					line = header_separater + line
				else
					line = line + header_separater
				end
			end

			line_count_in_tabular += 1

		end

		line.gsub!(/\\vspace.*$/){ |match| "" }

		line.gsub!(/^%.*$/){ |match| "" }

		line.gsub!(/\\section\*?\{(.+)\}/){ |match| independent_line = true; "## #{$1}" }
		line.gsub!(/\\subsection\*?\{(.+)\}/){ |match| independent_line = true; "### #{$1}" }
		line.gsub!(/\\subsubsection\*?\{(.+)\}/){ |match| independent_line = true; "#### #{$1}" }

		line.gsub!(/\\RABra/){ |match| " > " }

		line.gsub!(/\\begin\{itemize\}/){ |match| enter_block = true; block_type = "itemize"; "" }
		line.gsub!(/\\end\{itemize\}/){ |match| exit_block = true; "" }

		line.gsub!(/\\begin\{enumerate\}/){ |match| enter_block = true; block_type = "enumerate"; "" }
		line.gsub!(/\\end\{enumerate\}/){ |match| exit_block = true; "" }
		line.gsub!(/\\item/){ |match| (block_type == "enumerate") ?  "1. " : "- " }

		line.gsub!(/Table\\,\\ref\{[^\}]+\}/, "次表")
		line.gsub!(/Fig\.\\,\\ref\{[^\}]+\}/, "次図")

		line.gsub!(/\*([A-Za-z0-9_]+)\*\*/) { |match| "_#{$1}*_" }
		line.gsub!(/\*([A-Za-z0-9_]+)\<([A-Za-z0-9_]+)\>\*/) { |match| "*#{$1}\\<#{$2}>*" }

		if enter_block
			block_level += 1
		end

		if enter_block or independent_line
			lines += "\n" 
		end

		if not ignore
			lines += line
		end

 		if enter_block or independent_line
			lines += "\n"
		end

		if exit_block
			block_level -= 1
		end

	end

	ofp.puts(lines)

	cnt += 1

	ifp.close
	ofp.close

end
