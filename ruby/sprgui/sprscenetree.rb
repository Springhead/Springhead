include Fox

class SprSceneTree < FXTreeList
	#
	def initialize(owner)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		connect(SEL_SELECTED, method(:onSelected))
	end

	# ツリーを更新
	def update()

		# information of active scene
		fwscene = $sprapp.GetSdk().GetScene()
		# solids
		itemSolids = appendItem(nil, "solids")
		phscene = fwscene.GetPHScene()
		solids = phscene.GetSolids()
		solids.each do |s|
			appendItem(itemSolids, s.GetName(), nil, nil, s)
		end

		# joints

		# scene graph
		itemWorld = appendItem(nil, "world")
		grscene = fwscene.GetGRScene()
		world = grscene.GetWorld()
		appendChildNodes(world, itemWorld)

		$propertymanager.update(nil)

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
