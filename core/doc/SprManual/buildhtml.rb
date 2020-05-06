Dir.chdir("/export/home/WWW/docroots/springhead/doc/SprManual")

Dir.mkdir("tex_orig") unless File.directory?("tex_orig")

system("cp -a *.tex *.sty tex_orig")

Dir["tex_orig/*"].each do |file|
  system("/bin/cat #{file} | /bin/sed -e 's/\{sourcecode\}/\{verbatim\}/' | /usr/bin/nkf -w > #{file[9..-1]}")
end

system("/usr/bin/plastex main_html.tex")




