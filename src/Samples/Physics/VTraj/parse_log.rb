J = []
V = []
W = []
t = -1

open("iklog_3.txt") do |ifp|
  while line = ifp.gets
    if line =~ /\-\-\-\-\-/
      t += 1
    end
    
    if line =~ /\-\-\- v\[cp\:(.*)\]/
      cp = $1.to_i
      l = ifp.gets.gsub("(","").gsub(")","").chomp.strip.split(" ").map{|x| x.to_f}
      for j in 0...3
        V[t] = [] unless V[t]
        V[t][j+(cp*3)] = l[j]
      end
    end

    if line =~ /\-\-\- w\[nd\:(.*)\]/
      nd = $1.to_i
      l = ifp.gets.gsub("(","").gsub(")","").chomp.strip.split(" ").map{|x| x.to_f}
      for i in 0...(l.size)
        W[t] = [] unless W[t]
        W[t][i+(nd*2)] = l[i]
      end
    end

    if line =~ /\-\-\- J\[nd\:(.*)\]\[cp\:(.*)\]/
      nd = $1.to_i; cp = $2.to_i
      l = []
      l[0] = ifp.gets.gsub("(","").gsub(")","").chomp.strip.split(" ").map{|x| x.to_f}
      l[1] = ifp.gets.gsub("(","").gsub(")","").chomp.strip.split(" ").map{|x| x.to_f}
      l[2] = ifp.gets.gsub("(","").gsub(")","").chomp.strip.split(" ").map{|x| x.to_f}
      for j in 0...3
        for i in 0...(l[j].size)
          J[t] = [] unless J[t]
          J[t][j+(cp*3)] = [] unless J[t][j+(cp*3)]
          J[t][j+(cp*3)][i+(nd*2)] = l[j][i]
        end
      end
    end
  end
end

for t in 0...(J.size)
  cs = J[t].map{|r| r.size}.max
  for j in 0...(J[t].size)
    for i in 0...(cs)
      J[t][j][i] = 0 unless J[t][j][i]
    end
  end
end

for t in 0...(J.size)
  puts "J[#{t}] = ["
  J[t].each{|row| puts "  ["+row.map{|val| ("%+.05f" % val) }.join(", ")+"],"}
  puts "]"

  puts "V[#{t}] = ["
  puts "  "+V[t].map{|val| ("%+.05f" % val) }.join(", ")
  puts "]"

  puts "W[#{t}] = ["
  puts "  "+W[t].map{|val| ("%+.05f" % val) }.join(", ")
  puts "]"

  puts "----- ----- ----- ----- ----- "

  puts "mj = Matrix#{J[t].inspect}"
  puts "v  = Matrix[#{V[t].inspect}].t"
  puts "NV = #{V[t].size}"
  puts "w_ref  = Matrix[#{W[t].inspect}].t"
  puts "NW = #{W[t].size}"

  puts "----- ----- ----- ----- ----- ----- ----- ----- ----- ----- "
end

