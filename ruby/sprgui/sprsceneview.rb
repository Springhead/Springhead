include Fox

class SprPHSceneView < FXTreeList
	def initialize(owner, phscene)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@phscene = phscene
		connect(SEL_SELECTED, method(:onSelected))
		update
	end

	# ツリーを更新
	def update()
		# information of active scene
		phsdk   = $sprapp.GetSdk().GetPHSdk()
		
		# scene
		itemScene = appendItem(nil, "scene", nil, nil, @phscene)

		# solids
		solids = @phscene.GetSolids()
		itemSolids = appendItem(nil, "solids (#{solids.size})")
		solids.each do |s|
			appendItem(itemSolids, s.GetName(), nil, nil, s)
		end

		# shapes
		itemShapes = appendItem(nil, "shapes (#{phsdk.NShape()})")
		for i in 0..phsdk.NShape()-1
			shape = phsdk.GetShape(i)
			appendItem(itemShapes, shape.GetName, nil, nil, shape)
		end

		# joints
		joints = @phscene.GetJoints()
		itemJoints = appendItem(nil, "joints (#{joints.size})")
		joints.each do |j|
			appendItem(itemJoints, j.GetName(), nil, nil, j)
		end

	end

	# ツリーアイテム選択
	def onSelected(sender, sel, item)
		$propertymanager.update(item.data)
	end

end

class SprGRSceneView < FXTreeList
	def initialize(owner, grscene)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@grscene = grscene
		connect(SEL_SELECTED, method(:onSelected))
		update
	end
	
	def update
		# scene graph
		itemWorld = appendItem(nil, "world")
		world = @grscene.GetWorld()
		appendChildNodes(world, itemWorld)
	end

	# シーングラフのツリーを再帰的に構築
	def appendChildNodes(parent, item)
		children = parent.GetChildren()
		children.each do |c|
			citem = appendItem(item, c.GetName())

			# 子ノードがFrameならば再帰呼び出し
			fr = GRFrameIf.Cast(c)
			appendChildNodes(fr, citem) if fr
		end
	end

	# ツリーアイテム選択
	def onSelected(sender, sel, item)
		$propertymanager.update(item.data)
	end

end

class SprFWSceneView < FXTabBook
	def initialize(owner, scene)
		super(owner, nil, 0, TABBOOK_BOTTOMTABS)
		@scene = scene
		FXTabItem.new(self, '  physics  ', nil, TAB_BOTTOM)
		SprPHSceneView.new(self, @scene.GetPHScene())
		FXTabItem.new(self, '  graphics  ', nil, TAB_BOTTOM)
		SprGRSceneView.new(self, @scene.GetGRScene())
	end
end

class SprSceneView < FXTabBook
	#
	def initialize(owner)
		super(owner, nil, 0, TABBOOK_BOTTOMTABS | LAYOUT_FILL_X | LAYOUT_FILL_Y)
		@tabs = Array(0)
		@contents = Array(0)
	end

	#
	def addTab(scene)
		@tabs.push(FXTabItem.new(self, "  #{scene.GetName()}  ", nil, TAB_BOTTOM))
		@contents.push(SprFWSceneView.new(self, scene))
		recalc
	end

	def clear()
		children().each { |c| removeChild(c) }
		@tabs = Array(0)
		@contents = Array(0)
		puts children.size
	end

end
